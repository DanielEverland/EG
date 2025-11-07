// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

#include "json.hpp"
#include "DataStructrues/HashedString.h"
#include "DataStructrues/Vector.h"

class Chunk
{
public:
    Chunk();
    
    static constexpr uint8_t Width = 64;
    static constexpr uint8_t Height = 64;

    void SetTerrain(HashedString cellTypeName, IntVector2D localPos);

private:
    HashedString Terrain[Width * Height];
    std::unordered_map<size_t, uint8_t> TileSetIndexOffset;
};
