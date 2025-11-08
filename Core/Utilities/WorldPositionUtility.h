// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

#include "DataStructrues/Vector.h"

class WorldPositionUtility
{
public:
    static IntVector WorldPositionToChunkPosition(IntVector WorldPosition);
    static IntVector WorldSpaceToChunkSpace(IntVector WorldPosition);
};
