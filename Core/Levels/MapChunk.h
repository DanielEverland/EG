// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <memory>

#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"
#include "Rendering/Texture.h"
#include "Utilities/WorldPositionUtility.h"

struct TileInfo
{
    // Entity type of the tile. Several different appearances can use the same type (I.e. several different versions of a wall)
    HashedString EntityType;

    std::shared_ptr<Texture> TextureInfo;
    HashedString TextureName;
};

struct MapObjectInfo
{
    TileInfo Tile;
    IntVector2D ChunkSpacePosition;
};

class MapChunk
{
public:
    MapChunk();

    void SetTerrain(TileInfo&& cellData, IntVector2D localPos);
    bool TryGetCell(IntVector2D localPos, TileInfo& outCell) const;

    void AddObject(MapObjectInfo&& objectInfo);
    const std::vector<MapObjectInfo>& GetObjects() const { return Objects; }
    
    TileInfo Terrain[WorldPositionUtility::ChunkWidth * WorldPositionUtility::ChunkHeight];

private:
    std::vector<MapObjectInfo> Objects;
};
