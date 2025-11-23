// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/Vector.h"
#include "ECS/Entity.h"

class NavigationGraphTraverser
{
public:
    virtual ~NavigationGraphTraverser() = default;
    
    // Is it possible for this entity to move from a given position to another
    virtual bool IsValidMove(Entity entity, const IntVector& from, const IntVector& to) = 0;

    // This is called try since the algorithm will try to traverse cells that are not valid (I.e. walls)
    virtual bool TryGetTraversalDifficulty(const IntVector& from, const IntVector& to, uint8_t& outTraversalDifficulty) = 0;
};
