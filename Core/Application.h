#pragma once

#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Game.h"
#include "Input/InputReceiver.h"

class Application
{
public:
    static Application& Get()
    {
        return SingletonHelper::Impl<Application>();
    }

    void HandleInputEvent(const SDL_KeyboardEvent& key, bool isKeyDown);
    void Initialize();
    void Tick();
    void PrePresent();

private:
    InputReceiver InputHandler;
    
    void LoadContent();
};
