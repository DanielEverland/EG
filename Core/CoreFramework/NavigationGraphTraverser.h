// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/Vector.h"

class NavigationGraphTraverser
{
public:
    virtual ~NavigationGraphTraverser() = default;
    
    // A given position may be out of bounds due to being so far away a given chunk is not loaded yet
    virtual bool DoesWorldPositionExist(const IntVector& worldPosition) = 0;

    // This is called try since the algorithm will try to traverse cells that are not valid (I.e. walls)
    virtual bool TryGetTraversalDifficulty(const IntVector& from, const IntVector& to, uint8_t& outTraversalDifficulty) = 0;
};
