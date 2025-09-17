#include "Game.h"

#include "Level.h"
#include "MainGame/MainGameMode.h"
#include "Systems/RenderingSystem.h"

void Game::Initialize()
{
    CurrentLevel = CreateLevel();
    CurrentGameMode = std::make_shared<MainGameMode>();

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
    return std::make_shared<Level>();
}
