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

namespace SystemCategory
{
    enum Type
    {
        None = 0,
        GameTime = 1 << 0,
        RealTime = 1 << 1,
        RenderTime = 1 << 2
    };
}

class Game
{
public:
    static Game& Get()
    {
        return SingletonHelper::Impl<Game>();
    }
    
    void Initialize();
    void Tick();
    void StartRound();
    void PrePresent();
    
    Entity GetNextEntity();
    std::shared_ptr<Level> GetLevel() const { return CurrentLevel; }
    std::shared_ptr<GameMode> GetGameMode() const { return CurrentGameMode; }
    
    template <class T>
    requires std::derived_from<T, System>
    void CreateSystem(SystemCategory::Type category)
    {
        assert(category != SystemCategory::None);
        
        auto newSystem = std::make_shared<T>();
        AllSystems.push_back(newSystem);

        if ((category & SystemCategory::GameTime) != SystemCategory::None)
        {
            GameTimeSystems.Register(newSystem);
        }
        if ((category & SystemCategory::RealTime) != SystemCategory::None)
        {
            RealTimeSystems.Register(newSystem);
        }
        if ((category & SystemCategory::RenderTime) != SystemCategory::None)
        {
            RenderTimeSystems.Register(newSystem);
        }
    }
    
private:
    static inline Game* Instance = nullptr;
    Entity CurrentEntity = 0;

    // Ticks once per game round
    SystemScheduler GameTimeSystems;
    
    // Ticks once per frame
    SystemScheduler RealTimeSystems;

    // Ticks once per frame, after RealTimeSystems
    SystemScheduler RenderTimeSystems;
    
    std::vector<std::shared_ptr<System>> AllSystems;
    std::shared_ptr<Level> CurrentLevel;
    std::shared_ptr<GameMode> CurrentGameMode;
    
    std::shared_ptr<Level> CreateLevel();
    void UpdateCameraPosition();
};
