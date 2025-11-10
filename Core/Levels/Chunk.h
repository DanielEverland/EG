// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <memory>

#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"
#include "Rendering/Texture.h"

struct CellInfo
{
    // Type of the cell. Several different appearances can use the same type (I.e. several different versions of a wall)
    HashedString CellTypeName;

    std::shared_ptr<Texture> TextureInfo;
    HashedString TextureName;
};

class Chunk
{
public:
    Chunk();
    
    static constexpr uint8_t Width = 64;
    static constexpr uint8_t Height = 64;

    void SetTerrain(CellInfo&& cellData, IntVector2D localPos);
    bool TryGetCell(IntVector2D localPos, CellInfo& outCell) const;
    
    CellInfo Terrain[Width * Height];
};
