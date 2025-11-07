#include "LevelInstance.h"

#include <fstream>
#include <regex>

#include "Chunk.h"
#include "json.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "CoreFramework/AssetManager.h"
#include "DataStructrues/Vector.h"

using namespace std::filesystem;
using namespace nlohmann;
using namespace rapidxml;

/*Rect LevelInstance::GetSourceRectFromTileIdx(const std::string& tileSetName, uint16_t localIdx)
{
    assert(localIdx < TileSet.TileCount);
    return Rect((localIdx % TileSet.Columns) * TileSet.TileWidth, (localIdx / TileSet.Columns) * TileSet.TileHeight, TileSet.TileWidth, TileSet.TileHeight);
}

LevelInstance::TileSetData::TileData* LevelInstance::TryGetTileDataFromIdx(const std::string& tileSetName, uint16_t localIdx)
{
    if (!TileSet.Tiles.contains(localIdx))
        return nullptr;
    return &TileSet.Tiles[localIdx];
}*/

LevelInstance::LevelInstance() : LevelDirectory("N/A")
{
    
}

LevelInstance::LevelInstance(const std::string& levelDir) : LevelDirectory(levelDir)
{
}

void LevelInstance::LoadData()
{
    ParseWorldInfo();
    ParseTileSets();
    ParseAllChunks();
}

void LevelInstance::ParseWorldInfo()
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

void LevelInstance::ParseTileSets()
{
    std::hash<std::string> hasher;
    for (auto iter : directory_iterator(LevelDirectory))
    {
        if (iter.path().extension() != ".tsx")
            continue;

        TileSetData tileSet;
        
        file xmlFile(iter.path().generic_string().c_str());
        xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        auto tileSetNode = doc.first_node("tileset");
        assert(tileSetNode != nullptr);

        tileSet.TileWidth = std::stoi(tileSetNode->first_attribute("tilewidth", 0, false)->value());
        tileSet.TileHeight = std::stoi(tileSetNode->first_attribute("tileheight", 0, false)->value());
        tileSet.TileCount = std::stoi(tileSetNode->first_attribute("tilecount", 0, false)->value());
        tileSet.Columns = std::stoi(tileSetNode->first_attribute("columns", 0, false)->value());

        auto imageNode = tileSetNode->first_node("image");
        assert(imageNode != nullptr);
        std::string nameAttribute = imageNode->first_attribute("source", 0, false)->value();
        assert(!nameAttribute.empty());
        nameAttribute.insert(nameAttribute.begin(), '/');

        tileSet.AssetPath = LevelDirectory.generic_string() + nameAttribute;
        tileSet.AssetWidth = std::stoi(imageNode->first_attribute("width", 0, false)->value());
        tileSet.AssetHeight = std::stoi(imageNode->first_attribute("height", 0, false)->value());

        auto tileNode = tileSetNode->first_node("tile");
        while (tileNode != nullptr)
        {
            auto idVal = tileNode->first_attribute("id", 0, false);
            int32_t id = std::stoi(idVal->value());

            TileSetData::TileData tileData;
            tileData.TypeStr = HashedString(tileNode->first_attribute("type", 0, false)->value());

            tileSet.Tiles.emplace(id, tileData);

            tileNode = tileNode->next_sibling("tile");
        }

        std::string fileName = iter.path().filename().generic_string();
        LoadedTileSets.emplace(hasher(fileName), tileSet);
    }
}

void LevelInstance::ParseAllChunks()
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
                auto chunk = std::make_shared<Chunk>();
                
                assert(chunkData.is_object());
                assert(chunkData["width"].get<uint8_t>() == Chunk::Width);
                assert(chunkData["height"].get<uint8_t>() == Chunk::Height);

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
                    uint8_t xPosIndex = cellIdx % Chunk::Width;
                    uint8_t yPosIndex = cellIdx / Chunk::Width;
        
                    auto cellVal = *chunkDataIter;
                    assert(cellVal.is_number());

                    auto cellIdx = cellVal.get<uint16_t>();
                    
                    HashedString cellType;
                    // TODO: Support air
                    if (cellIdx == 0)
                    {
                        cellType = HashedString("Air");
                    }
                    else
                    {
                        // TODO: Implement support for multiple tilesets per map.
                        auto tileSetName = tileSets[0]["source"].get<std::string>();
                        size_t nameHash = strHasher(tileSetName);
                        auto tileSetEntry = LoadedTileSets.find(nameHash);
                        assert(tileSetEntry != LoadedTileSets.end());
                        const TileSetData& cellTileSetData = tileSetEntry->second;

                        // TODO: Multiple tilesets
                        // For now, just offset by 1 as they will always be 1-indexed
                        uint16_t actualId = cellIdx - 1;
                        auto tileEntry = cellTileSetData.Tiles.find(actualId);
                        assert(tileEntry != cellTileSetData.Tiles.end());

                        cellType = tileEntry->second.TypeStr;
                    }
                    chunk->SetTerrain(cellType, IntVector2D(xPosIndex, yPosIndex));
                }
            
                Chunks.emplace(chunkPos, chunk);
            }
        }
    }
}
