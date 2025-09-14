// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <utility>
#include "ECS/ComponentManager.h"

class Game;

class Level
{
public:
    explicit Level(std::shared_ptr<Game> game) : Game(std::move(game)) {}

    Entity CreateEntity() const;
    void DestroyEntity(Entity entity);
    ComponentManager& GetComponentManager() { return Components; }
    
private:
    std::shared_ptr<Game> Game;
    ComponentManager Components;
};
