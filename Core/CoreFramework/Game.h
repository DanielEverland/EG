// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>

#include "Renderer.h"
#include "ECS/Entity.h"
#include "ECS/SystemScheduler.h"
#include "Utilities/SingletonHelpers.h"

class GameMode;
class Application;
class Level;

enum class SystemCategory
{
    None = 0,
    GameTime = 1 << 0,
};

class Game
{
public:
    static Game& Get()
    {
        return SingletonHelper::Impl<Game>();
    }
    
    void Initialize();
    void Tick();
    
    Entity GetNextEntity();
    std::shared_ptr<Level> GetLevel() const { return CurrentLevel; }
    std::shared_ptr<GameMode> GetGameMode() const { return CurrentGameMode; }
    
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
    static inline Game* Instance = nullptr;
    Entity CurrentEntity = 0;
    
    SystemScheduler GameTimeSystems;
    
    std::vector<std::shared_ptr<System>> AllSystems;
    std::shared_ptr<Level> CurrentLevel;
    std::shared_ptr<GameMode> CurrentGameMode;
    
    std::shared_ptr<Level> CreateLevel();
};
