// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

#include "DataStructrues/Vector.h"

class WorldPositionUtility
{
public:
    static constexpr uint8_t ChunkWidth = 64;
    static constexpr uint8_t ChunkHeight = 64;
    
    static IntVector WorldPositionToChunkPosition(IntVector WorldPosition);
    static IntVector WorldSpaceToChunkSpace(IntVector WorldPosition);
    static IntVector ChunkSpaceToWorldPosition(const IntVector& ChunkPosition, const IntVector2D& LocalPosition);
    static bool IsChunkSpacePositionInChunk(const IntVector& pos);
    static int32_t GetTileIndex(const IntVector& chunkSpacePos);
};
