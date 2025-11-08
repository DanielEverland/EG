#include "WorldPositionUtility.h"

#include "Levels/Chunk.h"

IntVector WorldPositionUtility::WorldPositionToChunkPosition(IntVector WorldPosition)
{
    return IntVector
    {
        WorldPosition.X / Chunk::Width,
        WorldPosition.Y / Chunk::Height,
        WorldPosition.Z
    };
}

IntVector WorldPositionUtility::WorldSpaceToChunkSpace(IntVector WorldPosition)
{
    const IntVector chunkPos = WorldPositionToChunkPosition(WorldPosition);
    return IntVector
    {
        WorldPosition.X - chunkPos.X * Chunk::Width,
        WorldPosition.Y - chunkPos.Y * Chunk::Height,
        WorldPosition.Z - chunkPos.Z
    };
}
