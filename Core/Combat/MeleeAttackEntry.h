// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "AttackEntry.h"
#include "ECS/Entity.h"

class MeleeAttackEntry : public AttackEntry
{
public:
    Entity Target;

    void Execute(ComponentManager& compManager) override;
    std::vector<Entity> GetTargetEntities() const override;
};
