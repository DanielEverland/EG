#include "Application.h"
#include "json.hpp"

void Application::Initialize()
{
    LoadContent();
    
    GameInstance = std::make_shared<Game>(this);
    GameInstance->Initialize();
}

void Application::Tick()
{
    GameInstance->Tick();    
}

void Application::LoadContent()
{
    AssetManager::Get().Load();
}