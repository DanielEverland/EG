#include "Application.h"

void Application::Initialize(SDL_Renderer* rawRenderer)
{
    assert(rawRenderer != nullptr);
    
    GameInstance = std::make_shared<Game>(this);
    RendererInstance = std::make_shared<Renderer>(rawRenderer);
    
    GameInstance->Initialize();
}

void Application::Tick()
{
    GameInstance->Tick();    
}
