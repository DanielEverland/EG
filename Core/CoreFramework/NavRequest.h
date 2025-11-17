// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <memory>

#include "NavigationGraphTraverser.h"
#include "DataStructrues/Vector.h"

struct NavRequest
{
    IntVector StartPosition;
    IntVector TargetPosition;
    std::shared_ptr<NavigationGraphTraverser> Traverser = nullptr;
};
