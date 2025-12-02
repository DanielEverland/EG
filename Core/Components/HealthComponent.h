// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/Component.h"

#include "Factories/HealthComponentFactory.h"
REGISTER_COMP_FACTORY(HealthComponentFactory, "HealthComponent")

class HealthComponent : public Component
{
public:
    float Health = 10.f;
};
