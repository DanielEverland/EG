// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>

#include "ECS/Entity.h"
#include "ECS/SystemScheduler.h"

class Level;

enum class SystemCategory
{
    None = 0,
    GameTime = 1 << 0,
};

class Game : public std::enable_shared_from_this<Game>
{
public:
    void Initialize();
    void Tick();
    
    Entity GetNextEntity();
    std::shared_ptr<Level> GetLevel() const { return CurrentLevel; }
    
    template <class T>
    requires std::derived_from<T, System>
    void CreateSystem(SystemCategory category)
    {
        assert(category != SystemCategory::None);
        
        auto newSystem = std::make_shared<T>();
        AllSystems.push_back(newSystem);
        GameTimeSystems.Register(newSystem);
    }
    
private:
    SystemScheduler GameTimeSystems;
    std::vector<std::shared_ptr<System>> AllSystems;
    Entity CurrentEntity = 0;
    std::shared_ptr<Level> CurrentLevel;

    std::shared_ptr<Level> CreateLevel();
};
