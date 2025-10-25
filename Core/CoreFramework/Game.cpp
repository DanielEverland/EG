#include "Game.h"
#include "Camera.h"
#include "Level.h"
#include "Components/LocationComponent.h"
#include "ECS/EntityFactory.h"
#include "Input/Input.h"
#include "MainGame/MainGameMode.h"
#include "Systems/RenderingSystem.h"

void Game::Initialize()
{
    CurrentLevel = CreateLevel();
    CurrentGameMode = std::make_shared<MainGameMode>();

    CreateSystem<RenderingSystem>(SystemCategory::RenderTime);

    CurrentGameMode->Initialize();
}

void Game::Tick()
{
    Input::Get().ProcessInputBuffer();
    
    RealTimeSystems.Tick();
}

void Game::StartRound()
{
    GameTimeSystems.Tick();
}

void Game::PrePresent()
{
    RenderTimeSystems.Tick();
    
    UpdateCameraPosition();
}

Entity Game::GetNextEntity()
{
    return CurrentEntity++;
}

std::shared_ptr<Level> Game::CreateLevel()
{
    return std::make_shared<Level>();
}

void Game::UpdateCameraPosition()
{
    Entity possessedEntity = GetGameMode()->GetPossessedEntity();

    // No possessed entity may be valid. Camera should remain in place if so.
    if (possessedEntity == InvalidEntity)
        return;

    ComponentManager& CompManager = GetLevel()->GetComponentManager();
    auto Location = CompManager.TryGetComponent<LocationComponent>(possessedEntity);

    // TODO: Is this actually valid? Or should we assert?
    if (Location == nullptr)
        return;

    Camera::Get().SetPosition(Location->WorldLocation);
}