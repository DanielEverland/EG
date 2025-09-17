#include "Application.h"
#include "json.hpp"

void Application::Initialize()
{
    LoadContent();
    
    Game::Get().Initialize();
}

void Application::Tick()
{
    Game::Get().Tick();    
}

void Application::LoadContent()
{
    AssetManager::Get().Load();
}