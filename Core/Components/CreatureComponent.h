// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/Component.h"

#include "Factories/CreatureComponentFactory.h"
REGISTER_COMP_FACTORY(CreatureComponentFactory, "CreatureComponent")

class CreatureComponent : public Component
{
public:
};
