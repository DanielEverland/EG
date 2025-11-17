#include "MapChunk.h"

#include <cassert>

#include "Utilities/WorldPositionUtility.h"

MapChunk::MapChunk() : Terrain {  }
{
}

void MapChunk::SetTerrain(MapCellInfo&& cellData, IntVector2D localPos)
{
    assert(localPos.X >= 0 && localPos.X < WorldPositionUtility::ChunkWidth && localPos.Y >= 0 && localPos.Y < WorldPositionUtility::ChunkHeight);
    Terrain[localPos.X + localPos.Y * WorldPositionUtility::ChunkWidth] = std::move(cellData);
}

bool MapChunk::TryGetCell(IntVector2D localPos, MapCellInfo& outCell) const
{
    if (localPos.X >= 0 && localPos.Y >= 0 && localPos.X < WorldPositionUtility::ChunkWidth && localPos.Y < WorldPositionUtility::ChunkHeight)
    {
        outCell = Terrain[localPos.X + localPos.Y * WorldPositionUtility::ChunkWidth];
        return true;
    }
    return false;
}

/*std::shared_ptr<std::vector<Chunk>> Chunk::CreateFromJson(IntVector2D chunkPosPlane, const nlohmann::json& rawData)
{
    auto chunk = std::make_shared<Chunk>();
    
    auto tileSets = rawData["tilesets"];
    assert(tileSets.is_array());

    std::hash<std::string> stringHasher;
    for (auto tileSetsIter = tileSets.begin(); tileSetsIter != tileSets.end(); ++tileSetsIter)
    {
        auto tileSet = *tileSetsIter;
        assert(tileSet.is_object());

        auto firstGIDField = tileSet["firstgid"];
        assert(firstGIDField.is_number());
        uint8_t firstGID = firstGIDField.get<uint8_t>();
        // These are 1-indexed.
        assert(firstGID > 0);
        // They match the actual indices 1:1, so to convert to offset we have to subtract by 1.
        uint8_t indexOffset = firstGID - 1;

        auto sourceField = tileSet["source"];
        assert(sourceField.is_string());
        std::string sourceValue = sourceField.get<std::string>();
        size_t sourceHash = stringHasher(sourceValue);
        
        chunk->TileSetIndexOffset.emplace(sourceHash, indexOffset);
    }
    
    auto layers = rawData["layers"];
    assert(layers.is_array());

    for (auto layerIter = layers.begin(); layerIter != layers.end(); ++layerIter)
    {
        assert(rawData.is_object());
        assert(rawData["width"].get<uint8_t>() == Width);
        assert(rawData["height"].get<uint8_t>() == Height);

        std::string layerName = rawData["name"];
        assert(layerName.starts_with('L'));

        std::string depthStr = layerName.substr(1, layerName.length() - 1);
        int32_t depth = stoi(depthStr);
        
        uint8_t depthIndex = WorldPositionUtility::GetChunkDepthIndex(depth);

        uint16_t cellIdx = 0;
        auto data = rawData["data"];
        assert(data.is_array());
        for (auto iter = data.begin(); iter != data.end(); ++iter, ++cellIdx)
        {
            uint8_t xPosIndex = cellIdx % Width;
            uint8_t yPosIndex = cellIdx / Width;
        
            auto cellVal = *iter;
            assert(cellVal.is_number());

            auto cellIdx = cellVal.get<uint16_t>();
            chunk->Terrain[xPosIndex, yPosIndex] = HashedString(std::to_string(cellIdx));
        }
    }

    return chunk;
}*/
