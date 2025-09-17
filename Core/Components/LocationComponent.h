// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/Vector.h"
#include "ECS/Component.h"

class LocationComponent : public Component
{
public:
    IntVector2D WorldLocation;
};
