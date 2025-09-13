// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>

#include "ECS/Component.h"

class TestComponentA : public Component
{
public:
    int32_t A = 0;
    double B = 0;
};
