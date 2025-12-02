// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "ECS/Component.h"
#include "ECS/ComponentFactory.h"

#include "Factories/AttackerComponentFactory.h"
REGISTER_COMP_FACTORY(AttackerComponentFactory, "AttackerComponent")

class AttackEntry;

class AttackerComponent : public Component
{
public:

    // Systems submit attacks to this component, which will be consumed by AttackSystem
    std::shared_ptr<AttackEntry> PendingAttack;
};
