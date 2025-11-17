// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <memory>

#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"
#include "Rendering/Texture.h"
#include "Utilities/WorldPositionUtility.h"

struct MapCellInfo
{
    // Type of the cell. Several different appearances can use the same type (I.e. several different versions of a wall)
    HashedString CellTypeName;

    std::shared_ptr<Texture> TextureInfo;
    HashedString TextureName;
};

class MapChunk
{
public:
    MapChunk();

    void SetTerrain(MapCellInfo&& cellData, IntVector2D localPos);
    bool TryGetCell(IntVector2D localPos, MapCellInfo& outCell) const;
    
    MapCellInfo Terrain[WorldPositionUtility::ChunkWidth * WorldPositionUtility::ChunkHeight];
};
