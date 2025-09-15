#include "Game.h"

#include "Application.h"
#include "Level.h"
#include "MainGame/MainGameMode.h"
#include "Systems/RenderingSystem.h"

Game::Game(Application* application) : ApplicationPtr(application)
{
    assert(application != nullptr);
}

void Game::Initialize()
{
    CurrentLevel = CreateLevel();
    CurrentGameMode = std::make_shared<MainGameMode>(shared_from_this());

    CreateSystem<RenderingSystem>(SystemCategory::GameTime);

    CurrentGameMode->Initialize();
}

void Game::Tick()
{
    GameTimeSystems.Tick();
}

Entity Game::GetNextEntity()
{
    return CurrentEntity++;
}

std::shared_ptr<Level> Game::CreateLevel()
{
    return std::make_shared<Level>(shared_from_this());
}
