// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>
#include "ECS/Component.h"

class LocationComponent : public Component
{
public:
    int32_t X = 0;
    int32_t Y = 0;
};
