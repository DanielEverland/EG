// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <memory>

#include "NavigationGraphTraverser.h"
#include "DataStructrues/Vector.h"

struct NavRequest
{
    Entity EntityToMove;
    IntVector StartPosition;
    IntVector TargetPosition;
    IntVector PermittedTargetDistances = IntVector(0, 0, 0);
    std::shared_ptr<NavigationGraphTraverser> Traverser = nullptr;
};
