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
#include "Logging/Logger.h"
#include "Utilities/ConversionUtil.h"
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

void Map::TileSetReferences::AddRef(std::string source, uint32_t firstGid)
{
    References.push_back(TileSetReference(source, firstGid));

    std::sort(References.begin(), References.end(), [](const TileSetReference& a, const TileSetReference& b) -> bool
    {
        return a.MinGid < b.MinGid;
    });

    for (int i = 0; i < References.size(); ++i)
    {
        auto& ref = References[i];
        if (i == References.size() - 1)
        {
            ref.MaxGid = UINT32_MAX;
        }
        else
        {
            ref.MaxGid = References[i + 1].MinGid - 1;
        }
    }
}

Map::TileSetReference Map::TileSetReferences::GetTileset(uint32_t id) const
{
    for (const auto& ref : References)
    {
        if (ref.MinGid <= id && ref.MaxGid >= id)
        {
            return ref;
        }
    }
    assert(false && "This should never happen as we should always support [0; UINT32_MAX]");
    return {};
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
    TileInfo cellInfo = GetCellInfoFromWorldPosition(cellWorldPosition);
    return cellInfo.TextureInfo->SourceRect;
}

TileInfo Map::GetCellInfoFromWorldPosition(IntVector cellWorldPosition) const
{
    const IntVector chunkPos = WorldPositionUtility::WorldPositionToChunkPosition(cellWorldPosition);
    if (!Chunks.contains(chunkPos))
        return { };

    std::shared_ptr<const MapChunk> chunk = Chunks.at(chunkPos);
    
    const IntVector chunkSpacePos = WorldPositionUtility::WorldSpaceToChunkSpace(cellWorldPosition);
    assert(chunkSpacePos.X >= 0 && chunkSpacePos.Y >= 0 && chunkSpacePos.Z == 0);
    assert(chunkSpacePos.X < WorldPositionUtility::ChunkWidth && chunkSpacePos.Y < WorldPositionUtility::ChunkHeight);

    TileInfo cellInfo;
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

        std::string fileName = iter.path().filename().string();
        Logger::Log(Engine, Info, "Loading tileset [{}]", fileName);
        
        TileSetContainer& container = TilesetNameToContainer[fileName];
        
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
            if (!tileData.Name.IsValid())
            {
                Logger::Log(Engine, Warning, "Unable to load tile {} due to lack of valid 'Name' field (Its entity type). Skipping", tileData.TypeStr.ToString());
                tileNode = tileNode->next_sibling("tile");
                continue;
            }
            
            const bool succeeded = AssetManager::Get().TryRegisterTexture(tileData.Name, texture);
            assert(succeeded);

            TileInfo cellTemplate;
            cellTemplate.TextureInfo = texture;
            cellTemplate.EntityType = tileData.TypeStr;
            cellTemplate.TextureName = tileData.Name;
            container.emplace(id, cellTemplate);

            tileNode = tileNode->next_sibling("tile");
        }
    }
}

void Map::ParseAllChunks()
{
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

            TileSetReferences tileSetReferences;
            for (auto tilesetIter = tileSets.begin(); tilesetIter != tileSets.end(); ++tilesetIter)
            {
                assert(tilesetIter->is_object());

                uint32_t firstGid;
                if (!TryGetIntegerValue(*tilesetIter, "firstgid", firstGid, "Unable to get firstgid in tileset array while loading [{}]", pathStr))
                    continue;

                std::string source;
                if (!TryGetValue(*tilesetIter, "source", source, "Unable to get source in tileset array while loading [{}]", pathStr))
                    continue;

                tileSetReferences.AddRef(source, firstGid);
            }
            
            for (auto layerIter = layers.begin(); layerIter != layers.end(); ++layerIter)
            {
                auto chunkData = *layerIter;

                if (!chunkData.is_object())
                {
                    Logger::Log(Engine, Warning, "Encountered error while parsing chunk layer in [{}] - json is not object:\n{}", pathStr, chunkData.dump());
                    continue;
                }

                auto typeEntry = chunkData.find("type");
                if (typeEntry == chunkData.end())
                {
                    Logger::Log(Engine, Warning, "Layer [{}] does not have a type field\n{}", pathStr, chunkData.dump());
                    continue;
                }
                std::string type = typeEntry->get<std::string>();

                if (type == "tilelayer")
                {
                    ParseTileLayer(chunkData, mapPos, tileSetReferences);
                }
                else if (type == "objectgroup")
                {
                    ParseObjectLayer(chunkData, mapPos, tileSetReferences);
                }
                else
                {
                    Logger::Log(Engine, Warning, "Unable to parse layer [{}] since its type [{}] is not known", pathStr, type);
                }
            }
        }
    }
}

void Map::ParseObjectLayer(json& chunkData, const IntVector2D& mapPos, const TileSetReferences& tileSetReferences)
{    
    std::string layerName = chunkData["name"];
    assert(layerName.starts_with("OL"));

    std::string depthStr = layerName.substr(2, layerName.length() - 2);
    std::optional<int32_t> chunkZDepth = ConversionUtil::TryToInteger<int32_t>(depthStr);
    if (!chunkZDepth.has_value())
    {
        Logger::Log(Engine, Warning, "Failed to parse layer name z-depth [{}]. Object layers must use 'OL' prefix followed by z-depth integer", depthStr);
        return;
    }
    
    IntVector chunkPos(mapPos.X, mapPos.Y, chunkZDepth.value());
    std::shared_ptr<MapChunk> chunk = GetOrCreateChunk(chunkPos);
    
    auto data = chunkData["objects"];
    assert(data.is_array());
    for (auto objectDataIter = data.begin(); objectDataIter != data.end(); ++objectDataIter)
    {
        int32_t x, y;
        if (!TryGetIntegerValue(*objectDataIter, "x", x, "Unable to parse 'x' in layer [{}]", layerName)
            || !TryGetIntegerValue(*objectDataIter, "y", y, "Unable to parse 'y' in layer [{}]", layerName))
            continue;

        int32_t width, height;
        if (!TryGetIntegerValue(*objectDataIter, "width", width, "Unable to parse 'width' in layer [{}]", layerName)
            || !TryGetIntegerValue(*objectDataIter, "height", height, "Unable to parse 'height' in layer [{}]", layerName))
            continue;

        int32_t id;
        if (!TryGetIntegerValue(*objectDataIter, "gid", id, "Unable to parse 'gid' in layer [{}]", layerName))
            continue;

        TileInfo tileInfo;
        if (!TryGetMapCell(tileSetReferences, id, tileInfo))
        {
            Logger::Log(Engine, Warning, "Unable to convert cell id [{}] to cell info", id);
            continue;
        }

        MapObjectInfo objectInfo;
        // TODO: So far, seems like it's offset by one along the vertical axis. Keep an eye on this.
        objectInfo.ChunkSpacePosition = IntVector2D(x / width, y / height - 1);
        objectInfo.Tile = tileInfo;
        chunk->AddObject(std::move(objectInfo));
    }
}

void Map::ParseTileLayer(json& chunkData, const IntVector2D& mapPos, const TileSetReferences& tileSetReferences)
{
    if (chunkData["width"].get<uint8_t>() != WorldPositionUtility::ChunkWidth || chunkData["height"].get<uint8_t>() != WorldPositionUtility::ChunkHeight)
    {
        Logger::Log(Engine, Warning, "Layer width and height does not match expected size\n{}", chunkData.dump());
        return;
    }
    
    std::string layerName = chunkData["name"];
    assert(layerName.starts_with('L'));

    std::string depthStr = layerName.substr(1, layerName.length() - 1);
    // TODO: Support failed parse
    int32_t chunkZDepth = stoi(depthStr);

    IntVector chunkPos(mapPos.X, mapPos.Y, chunkZDepth);
    std::shared_ptr<MapChunk> chunk = GetOrCreateChunk(chunkPos);
                
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

        TileInfo newCell;
                    
        if (cellIdx == 0)
        {
            newCell.EntityType = HashedString("Null");
        }
        else
        {
            if (!TryGetMapCell(tileSetReferences, cellIdx, newCell))
            {
                Logger::Log(Engine, Warning, "Unable to convert cell id [{}] to cell info", cellIdx);
                continue;
            }
        }
        chunk->SetTerrain(std::move(newCell), IntVector2D(xPosIndex, yPosIndex));
    }
}

bool Map::TryGetMapCell(const TileSetReferences& tileSetReferences, uint32_t id, TileInfo& info)
{
    TileSetReference tileSetName = tileSetReferences.GetTileset(id);
    const TileSetContainer& container = TilesetNameToContainer[tileSetName.Source];
    auto textureEntry = container.find(id - tileSetName.MinGid);
    if(textureEntry == container.end())
        return false;
    
    info = textureEntry->second;
    return true;
}
std::shared_ptr<MapChunk> Map::GetOrCreateChunk(const IntVector& chunkPos)
{
    if (Chunks.contains(chunkPos))
        return Chunks[chunkPos];
    
    return Chunks[chunkPos] = std::make_shared<MapChunk>();
}
