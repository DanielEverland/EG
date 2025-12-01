#pragma once

#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Game.h"
#include "Input/InputReceiver.h"

class EntityFactory;

class Application
{
public:
    static Application& Get()
    {
        return SingletonHelper::Impl<Application>();
    }

    std::shared_ptr<EntityFactory> GetEntityFactory() const { return EntityTemplateFactory; }

    uint16_t GameTimeMs;
    uint16_t RenderTimeMs;
    
    void HandleInputEvent(const SDL_KeyboardEvent& key, bool isKeyDown);
    void Initialize();
    void Tick();
    void PrePresent();

private:
    InputReceiver InputHandler;
    std::shared_ptr<EntityFactory> EntityTemplateFactory;
    
    void LoadContent();
};
