#include "Application.h"
#include "json.hpp"

void Application::Initialize(SDL_Renderer* rawRenderer)
{
    assert(rawRenderer != nullptr);

    LoadContent(rawRenderer);
    
    GameInstance = std::make_shared<Game>(this);
    RendererInstance = std::make_shared<Renderer>(rawRenderer, AssetManagerInstance);
    
    GameInstance->Initialize();
}

void Application::Tick()
{
    GameInstance->Tick();    
}

void Application::LoadContent(SDL_Renderer* rawRenderer)
{
    AssetManagerInstance.Load(rawRenderer);
}