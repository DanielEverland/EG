#pragma once

#include "CoreFramework/AssetManager.h"
#include "CoreFramework/Game.h"

class Application
{
public:
    void Initialize(SDL_Renderer* rawRenderer);
    void Tick();
    std::shared_ptr<Renderer> GetRenderer() { return RendererInstance; }

private:
    AssetManager AssetManagerInstance;
    
    std::shared_ptr<Game> GameInstance;
    std::shared_ptr<Renderer> RendererInstance;

    void LoadContent(SDL_Renderer* rawRenderer);
};
