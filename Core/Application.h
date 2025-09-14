#pragma once

#include "CoreFramework/Game.h"

class Application
{
public:
    void Initialize(SDL_Renderer* rawRenderer);
    void Tick();
    std::shared_ptr<Renderer> GetRenderer() { return RendererInstance; }

private:
    std::shared_ptr<Game> GameInstance;
    std::shared_ptr<Renderer> RendererInstance;
};
