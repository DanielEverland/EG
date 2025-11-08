// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>
#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"

struct CellInfo
{
    // Type of the cell. Several different appearances can use the same type (I.e. several different versions of a wall)
    HashedString CellTypeName;

    // The id of the tileset
    size_t TileSetId;
    
    // The id of the tile within the tileset 
    uint16_t TileId;
};

class Chunk
{
public:
    Chunk();
    
    static constexpr uint8_t Width = 64;
    static constexpr uint8_t Height = 64;

    void SetTerrain(CellInfo&& cellData, IntVector2D localPos);
    bool TryGetCell(IntVector2D localPos, CellInfo& outCell) const;

private:
    CellInfo Terrain[Width * Height];
};
