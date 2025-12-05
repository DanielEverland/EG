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
    auto diff = std::chrono::duration_cast< std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - lastFrameTime);
    lastFrameTime = std::chrono::high_resolution_clock::now();
    Time += static_cast<double>(diff.count()) / 1000000.0;
    
    Input::Get().ProcessInputBuffer();
    
    RealTimeSystems.Tick();
    
    CurrentLevel->ConsumeEntitiesMarkedDestroy();
}

void Game::StartRound()
{
    GameTimeSystems.Tick();
}

void Game::PrePresent()
{
    UpdateCameraPosition();
    RenderTimeSystems.Tick();
}

Entity Game::GetNextEntity()
{
    return ++CurrentEntity;
}

std::shared_ptr<Level> Game::CreateLevel()
{
    return std::make_shared<Level>();
}

void Game::UpdateCameraPosition()
{
    auto& camera = Camera::Get();

    if (!camera.GetSnapToPossessed())
        return;
    
    Entity possessedEntity = GetGameMode()->GetPossessedEntity();

    // No possessed entity may be valid. Camera should remain in place if so.
    if (possessedEntity == InvalidEntity)
        return;

    ComponentManager& CompManager = GetLevel()->GetComponentManager();
    auto Location = CompManager.TryGetComponent<LocationComponent>(possessedEntity);

    // TODO: Is this actually valid? Or should we assert?
    if (Location == nullptr)
        return;

    camera.SetPosition(Location->GetLocation());
}