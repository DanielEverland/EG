// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <memory>

#include "Primitives/NavPath.h"

struct NavResult
{
    bool Succeeded = false;
    std::list<IntVector> Path;
};
