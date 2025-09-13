#include "Game.h"

#include "Level.h"
#include "Systems/RenderingSystem.h"

void Game::Initialize()
{
    CurrentLevel = CreateLevel();

    CreateSystem<RenderingSystem>(SystemCategory::GameTime);
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
