#include "Application.h"

#include "ECS/EntityFactory.h"
#include "Input/Input.h"
#include "Logging/Logger.h"

void Application::Initialize()
{
    Logger::Init();
    
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
    Logger::Log(Engine, Info, "Loading content...");
    AssetManager::Get().Load();

    EntityTemplateFactory = std::make_shared<EntityFactory>();
    EntityTemplateFactory->Load();
}

void Application::HandleInputEvent(const SDL_KeyboardEvent& key, bool isKeyDown)
{
    Input::Get().PushOntoInputBuffer(key.key, isKeyDown);
}