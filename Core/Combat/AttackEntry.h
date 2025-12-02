// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "ECS/Entity.h"

class HealthComponent;
class ComponentManager;

class AttackEntry
{
public:
    virtual ~AttackEntry() = default; 

    virtual void Execute(ComponentManager& compManager) = 0;
    [[nodiscard]] virtual std::vector<Entity> GetTargetEntities() const = 0;
};
