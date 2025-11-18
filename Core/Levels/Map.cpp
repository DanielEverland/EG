#include "Map.h"

#include <fstream>
#include <regex>
#include <filesystem>
#include <iostream>
#include <unordered_set>

#include "MapChunk.h"
#include "json.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Renderer.h"
#include "DataStructrues/Vector.h"
#include "Utilities/WorldPositionUtility.h"

using namespace std::filesystem;
using namespace nlohmann;
using namespace rapidxml;

Map::Map() : LevelDirectory("N/A")
{
    
}

Map::Map(const std::string& levelDir) : LevelDirectory(levelDir)
{
}

Rect Map::TileSetData::IdToRect(uint16_t id) const
{
    return Rect
    {
        id % Columns * TileWidth,
        id / Columns * TileHeight,
        TileWidth,
        TileHeight
    };
}

Rect Map::GetSourceRectFromWorldPosition(IntVector cellWorldPosition) const
{
    MapCellInfo cellInfo = GetCellInfoFromWorldPosition(cellWorldPosition);
    return cellInfo.TextureInfo->SourceRect;
}

MapCellInfo Map::GetCellInfoFromWorldPosition(IntVector cellWorldPosition) const
{
    const IntVector chunkPos = WorldPositionUtility::WorldPositionToChunkPosition(cellWorldPosition);
    if (!Chunks.contains(chunkPos))
        return { };

    std::shared_ptr<const MapChunk> chunk = Chunks.at(chunkPos);
    
    const IntVector chunkSpacePos = WorldPositionUtility::WorldSpaceToChunkSpace(cellWorldPosition);
    assert(chunkSpacePos.X >= 0 && chunkSpacePos.Y >= 0 && chunkSpacePos.Z == 0);
    assert(chunkSpacePos.X < WorldPositionUtility::ChunkWidth && chunkSpacePos.Y < WorldPositionUtility::ChunkHeight);

    MapCellInfo cellInfo;
    chunk->TryGetCell(static_cast<IntVector2D>(chunkSpacePos), cellInfo);

    return cellInfo;
}

void Map::LoadData()
{
    ParseWorldInfo();
    ParseTileSets();
    ParseAllChunks();
}

void Map::ParseWorldInfo()
{
    path worldPath;
    for (auto iter : directory_iterator(LevelDirectory))
    {
        if (iter.path().extension() == ".world")
        {
            worldPath = iter.path();
            break;
        }
    }
    assert(worldPath.empty() == false);

    std::ifstream worldStream(worldPath);
    json worldData = json::parse(worldStream);
    
    assert(worldData.is_null() == false);

    json pattern = worldData["patterns"][0];
    assert(pattern.is_null() == false);
    
    World.MapPatternStr = pattern["regexp"].get<std::string>();
    World.MapPatternRegex = std::regex(World.MapPatternStr);
}

void Map::ParseTileSets()
{
    for (auto iter : directory_iterator(LevelDirectory))
    {
        if (iter.path().extension() != ".tsx")
            continue;
        
        file xmlFile(iter.path().generic_string().c_str());
        xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        auto tileSetNode = doc.first_node("tileset");
        assert(tileSetNode != nullptr);

        auto TileWidth = std::stoi(tileSetNode->first_attribute("tilewidth")->value());
        auto TileHeight = std::stoi(tileSetNode->first_attribute("tileheight")->value());
        auto TileCount = std::stoi(tileSetNode->first_attribute("tilecount")->value());
        auto Columns = std::stoi(tileSetNode->first_attribute("columns")->value());

        auto imageNode = tileSetNode->first_node("image");
        assert(imageNode != nullptr);
        std::string nameAttribute = imageNode->first_attribute("source")->value();
        assert(!nameAttribute.empty());
        nameAttribute.insert(nameAttribute.begin(), '/');

        auto AssetPath = LevelDirectory.generic_string() + nameAttribute;
        auto AssetWidth = std::stoi(imageNode->first_attribute("width")->value());
        auto AssetHeight = std::stoi(imageNode->first_attribute("height")->value());

        std::shared_ptr<Tileset> tileset = AssetManager::Get().LoadTileset(AssetPath);

        auto tileNode = tileSetNode->first_node("tile");
        while (tileNode != nullptr)
        {
            auto texture = std::make_shared<Texture>();
            texture->Tileset = tileset;
            
            auto idVal = tileNode->first_attribute("id");
            int32_t id = std::stoi(idVal->value());

            texture->SourceRect = Rect
            {
                id % Columns * TileWidth,
                id / Columns * TileHeight,
                TileWidth,
                TileHeight
            };

            TileSetData::TileData tileData;
            tileData.TypeStr = HashedString(tileNode->first_attribute("type")->value());
            
            if (auto propertiesNode = tileNode->first_node("properties"))
            {
                auto propertyNode = propertiesNode->first_node("property");
                while (propertyNode != nullptr)
                {
                    std::string propertyName = propertyNode->first_attribute("name")->value();
                    std::string propertyValue = propertyNode->first_attribute("value")->value();
                    if (propertyName == "Name")
                    {
                        tileData.Name = HashedString(propertyValue);
                        break;
                    }
                    propertyNode = propertiesNode->next_sibling("property");
                }
            }
            assert(tileData.Name.IsValid());
            
            const bool succeeded = AssetManager::Get().TryRegisterTexture(tileData.Name, texture);
            assert(succeeded);

            MapCellInfo cellTemplate;
            cellTemplate.TextureInfo = texture;
            cellTemplate.CellTypeName = tileData.TypeStr;
            cellTemplate.TextureName = tileData.Name;
            IdToCellTemplate.emplace(id, cellTemplate);

            tileNode = tileNode->next_sibling("tile");
        }

        std::string fileName = iter.path().filename().generic_string();
    }
}

void Map::ParseAllChunks()
{
    auto strHasher = std::hash<std::string>();
    for (auto iter : directory_iterator(LevelDirectory))
    {
        std::string pathStr = iter.path().generic_string();
        std::smatch pathMatch;
        if (std::regex_search(pathStr, pathMatch, World.MapPatternRegex) && pathMatch.size() == 3)
        {
            std::ifstream mapStream(pathStr);
            json mapData = json::parse(mapStream);
            
            IntVector2D mapPos;
            
            std::string firstStr = pathMatch[1].str();
            std::string secondStr = pathMatch[2].str();

            mapPos.X = std::stoi(firstStr);
            mapPos.Y = std::stoi(secondStr);

            auto layers = mapData["layers"];
            assert(layers.is_array());
            auto tileSets = mapData["tilesets"];
            assert(tileSets.is_array());
            for (auto layerIter = layers.begin(); layerIter != layers.end(); ++layerIter)
            {
                auto chunkData = *layerIter;
                auto chunk = std::make_shared<MapChunk>();
                
                assert(chunkData.is_object());
                assert(chunkData["width"].get<uint8_t>() == WorldPositionUtility::ChunkWidth);
                assert(chunkData["height"].get<uint8_t>() == WorldPositionUtility::ChunkHeight);

                std::string layerName = chunkData["name"];
                assert(layerName.starts_with('L'));

                std::string depthStr = layerName.substr(1, layerName.length() - 1);
                // TODO: Support failed parse
                int32_t chunkZDepth = stoi(depthStr);

                IntVector chunkPos(mapPos.X, mapPos.Y, chunkZDepth);
                
                uint16_t cellIdx = 0;
                auto data = chunkData["data"];
                assert(data.is_array());
                for (auto chunkDataIter = data.begin(); chunkDataIter != data.end(); ++chunkDataIter, ++cellIdx)
                {                    
                    uint8_t xPosIndex = cellIdx % WorldPositionUtility::ChunkWidth;
                    uint8_t yPosIndex = cellIdx / WorldPositionUtility::ChunkWidth;
        
                    auto cellVal = *chunkDataIter;
                    assert(cellVal.is_number());

                    auto cellIdx = cellVal.get<uint16_t>();

                    MapCellInfo newCell;
                    // TODO: Support air
                    if (cellIdx == 0)
                    {
                        newCell.CellTypeName = HashedString("Air");
                    }
                    else
                    {
                        // Technically 1-indexed. With more tilesets you would have to offset, but for now this is ok.
                        auto textureEntry = IdToCellTemplate.find(cellIdx - 1);
                        assert(textureEntry != IdToCellTemplate.end());
                        newCell = textureEntry->second;
                    }
                    chunk->SetTerrain(std::move(newCell), IntVector2D(xPosIndex, yPosIndex));
                }
            
                Chunks.emplace(chunkPos, chunk);
            }
        }
    }
}
