#include "Application.h"
#include "Input/Input.h"

void Application::Initialize()
{
    LoadContent();
    
    Game::Get().Initialize();
}

void Application::Tick()
{
    Game::Get().Tick();    
}

void Application::PrePresent()
{
    Game::Get().PrePresent();
}

void Application::LoadContent()
{
    AssetManager::Get().Load();
}

void Application::HandleInputEvent(const SDL_KeyboardEvent& key, bool isKeyDown)
{
    Input::Get().PushOntoInputBuffer(key.key, isKeyDown);
}