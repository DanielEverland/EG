#include "MapChunk.h"

#include <cassert>

#include "Utilities/WorldPositionUtility.h"

MapChunk::MapChunk() : Terrain {  }
{
}

void MapChunk::SetTerrain(TileInfo&& cellData, IntVector2D localPos)
{
    assert(localPos.X >= 0 && localPos.X < WorldPositionUtility::ChunkWidth && localPos.Y >= 0 && localPos.Y < WorldPositionUtility::ChunkHeight);
    Terrain[localPos.X + localPos.Y * WorldPositionUtility::ChunkWidth] = std::move(cellData);
}

bool MapChunk::TryGetCell(IntVector2D localPos, TileInfo& outCell) const
{
    if (localPos.X >= 0 && localPos.Y >= 0 && localPos.X < WorldPositionUtility::ChunkWidth && localPos.Y < WorldPositionUtility::ChunkHeight)
    {
        outCell = Terrain[localPos.X + localPos.Y * WorldPositionUtility::ChunkWidth];
        return true;
    }
    return false;
}

void MapChunk::AddObject(MapObjectInfo&& objectInfo)
{
    Objects.push_back(std::move(objectInfo));
}
