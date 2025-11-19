#include "MainGameMode.h"

#include <unordered_set>
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"
#include "Input/Input.h"
#include "Input/InputAction.h"
#include "Levels/Map.h"
#include "Systems/CreatureAISystem.h"
#include "Systems/MovementSystem.h"
#include "Utilities/DirectoryHelpers.h"

void MainGameMode::Initialize()
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();
    
    LoadMap(level);
    CreatePlayer(level, componentManager);
    SpawnEnemies(level, componentManager);
    RegisterInput();

    Game::Get().CreateSystem<CreatureAISystem>(SystemCategory::GameTime);
    Game::Get().CreateSystem<MovementSystem>(SystemCategory::GameTime);
}

void MainGameMode::LoadMap(std::shared_ptr<Level> level)
{
    std::filesystem::path p = DirectoryHelpers::GetContentPath("Levels/Level_01");
    std::string pString = p.generic_string();
    auto levelData = Map(pString);
    levelData.LoadData();

    for (const std::pair<const TemplatedVector<int>, std::shared_ptr<MapChunk>>& pair : levelData.GetChunks())
    {        
        level->LoadChunk(pair.first, pair.second);
    }
}

void MainGameMode::CreatePlayer(std::shared_ptr<Level> level, ComponentManager& componentManager)
{
    Entity playerEntity = level->CreateEntity("Player");
    SetPossessedEntity(playerEntity);
    auto& playerLocation = level->GetComponentManager().GetComponentChecked<LocationComponent>(playerEntity);
    playerLocation.SetLocation(playerEntity, IntVector(12, 12, 0));
}

void MainGameMode::SpawnEnemies(std::shared_ptr<Level> level, ComponentManager& componentManager)
{
    IntVector2D enemySpawnPositions[] = {
        { 10, 5 },
        { 5, 10 },
        { 11, 12 }
    };
    for (auto enemyPosition : enemySpawnPositions)
    {
        Entity enemyEntity = level->CreateEntity("Ghost");
        auto& enemyLocation = componentManager.GetComponentChecked<LocationComponent>(enemyEntity);

        enemyLocation.SetLocation(enemyEntity, enemyPosition);
    }
}

void MainGameMode::RegisterInput()
{    
    // Horizontal movement
    {
        const auto action = std::make_shared<AxisInputAction>();
        action->AddKeycodeAxis(SDLK_D, 1, InputEventType::DownOrHeld);
        action->AddKeycodeAxis(SDLK_A, -1, InputEventType::DownOrHeld);
        action->AddKeycodeAxis(SDLK_RIGHT, 1, InputEventType::DownOrHeld);
        action->AddKeycodeAxis(SDLK_LEFT, -1, InputEventType::DownOrHeld);
        
        action->AddCallback(std::bind(&MainGameMode::HandleMovementInput, this, std::placeholders::_1, true));

        Input::Get().RegisterAction(action);
    }

    // Vertical movement
    {
        const auto action = std::make_shared<AxisInputAction>();
        action->AddKeycodeAxis(SDLK_W, -1, InputEventType::DownOrHeld);
        action->AddKeycodeAxis(SDLK_S, 1, InputEventType::DownOrHeld);
        action->AddKeycodeAxis(SDLK_UP, -1, InputEventType::DownOrHeld);
        action->AddKeycodeAxis(SDLK_DOWN, 1, InputEventType::DownOrHeld);
        
        action->AddCallback(std::bind(&MainGameMode::HandleMovementInput, this, std::placeholders::_1, false));

        Input::Get().RegisterAction(action);
    }
}
void MainGameMode::HandleMovementInput(int32_t value, bool bIsHorizontal)
{
    Game game = Game::Get();
    Entity possessedEntity = game.GetGameMode()->GetPossessedEntity();
    if (possessedEntity == InvalidEntity)
        return;

    auto& movementComponent = game.GetLevel()->GetComponentManager().GetComponentChecked<MovementComponent>(possessedEntity);
    const auto& locationComponent = game.GetLevel()->GetComponentManager().GetComponentChecked<LocationComponent>(possessedEntity);

    movementComponent.TargetLocation = locationComponent.GetLocation();
    if (bIsHorizontal)
    {
        movementComponent.TargetLocation.X += value;
    }
    else
    {
        movementComponent.TargetLocation.Y += value;
    }

    auto movementSystem = game.GetSystem<MovementSystem>();
    if (movementSystem->IsValidMove(possessedEntity, locationComponent.GetLocation(), movementComponent.TargetLocation))
    {
        game.StartRound();
    }
}
