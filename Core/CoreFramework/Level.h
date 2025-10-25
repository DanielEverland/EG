// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <utility>

#include "DataStructrues/Vector.h"
#include "ECS/ComponentManager.h"

class Game;

class Level
{
public:
    Entity CreateEntity() const;
    Entity CreateEntity(const std::string& templateId) const;
    Entity CreateEntity(const std::string& templateId, const IntVector2D& position) const;
    
    void DestroyEntity(Entity entity);
    ComponentManager& GetComponentManager() { return Components; }
    
private:
    ComponentManager Components;
};
