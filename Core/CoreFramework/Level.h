// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <utility>
#include "ECS/ComponentManager.h"

class Game;

class Level
{
public:
    Entity CreateEntity() const;
    void DestroyEntity(Entity entity);
    ComponentManager& GetComponentManager() { return Components; }
    
private:
    ComponentManager Components;
};
