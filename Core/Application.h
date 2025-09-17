#pragma once

#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Game.h"

class Application
{
public:
    static Application& Get()
    {
        return SingletonHelper::Impl<Application>();
    }
    
    void Initialize();
    void Tick();

private:    
    void LoadContent();
};
