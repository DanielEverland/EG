#pragma once

#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Game.h"

class Application
{
public:
    static Application& Get()
    {
        if (Instance == nullptr)
        {
            Instance = new Application();
        }
        return *Instance;
    }
    
    void Initialize();
    void Tick();

private:
    Application() = default;
    
    static inline Application* Instance;
    
    std::shared_ptr<Game> GameInstance;

    void LoadContent();
};
