#include "MainGameMode.h"

#include <unordered_set>

#include "Components/CollisionComponent.h"
#include "Components/CreatureComponent.h"
#include "Components/LocationComponent.h"
#include "Components/MovementComponent.h"
#include "Components/TextureRendererComponent.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"
#include "ECS/EntityFactory.h"
#include "Input/Input.h"
#include "Input/InputAction.h"
#include "Levels/LevelInstance.h"
#include "Systems/CreatureAISystem.h"
#include "Systems/MovementSystem.h"
#include "Utilities/DirectoryHelpers.h"
#include "Utilities/WorldPositionUtility.h"

void MainGameMode::Initialize()
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();

    Entity playerEntity = CreateCreature();
    SetPossessedEntity(playerEntity);
    
    auto& playerLocation = componentManager.GetComponentChecked<LocationComponent>(playerEntity);
    auto& renderComp = componentManager.GetComponentChecked<TextureRendererComponent>(playerEntity);
    
    playerLocation.WorldLocation = IntVector2D(5, 5);

    renderComp.TextureName = HashedString("Player");
    renderComp.Order = DrawCallOrder::Foreground;
    
    std::filesystem::path p = DirectoryHelpers::GetContentPath("Levels/Level_01");
    std::string pString = p.generic_string();
    auto levelData = LevelInstance(pString);
    levelData.LoadData();

    std::unordered_set<IntVector> positions;
    for (const std::pair<const TemplatedVector<int>, std::shared_ptr<Chunk>>& pair : levelData.GetChunks())
    {
        if (pair.first.Z != 0)
            continue;
        
        for (int x = 0; x < Chunk::Width; ++x)
        {
            for (int y = 0; y < Chunk::Height; ++y)
            {
                const CellInfo& cellInfo = pair.second->Terrain[x + y * Chunk::Width];
                IntVector position = WorldPositionUtility::ChunkSpaceToWorldPosition(pair.first, IntVector2D(x, y));
                assert(!positions.contains(position));
                positions.emplace(position);
                Entity entity = level->CreateEntity(cellInfo.CellTypeName, IntVector2D(position.X, position.Y));
                
                auto& renderer = Game::Get().GetLevel()->GetComponentManager().GetComponentChecked<TextureRendererComponent>(entity);
                renderer.TextureName = cellInfo.TextureName;
            }
        }
    }

    IntVector2D enemySpawnPositions[] = {
        { 10, 5 },
        { 5, 10 },
        { 11, 12 }
    };
    for (auto enemyPosition : enemySpawnPositions)
    {
        Entity enemyEntity = CreateCreature();
        auto& enemyLocation = componentManager.GetComponentChecked<LocationComponent>(enemyEntity);
        auto& enemyRenderComp = componentManager.GetComponentChecked<TextureRendererComponent>(enemyEntity);
        auto& enemyMovementComp = componentManager.GetComponentChecked<MovementComponent>(enemyEntity);

        enemyLocation.WorldLocation = enemyPosition;

        enemyRenderComp.TextureName = HashedString("Enemy");
        enemyRenderComp.Order = DrawCallOrder::Foreground;

        enemyMovementComp.MovementSpeed = 0.75f;
    }
    
    RegisterInput();

    Game::Get().CreateSystem<CreatureAISystem>(SystemCategory::GameTime);
    Game::Get().CreateSystem<MovementSystem>(SystemCategory::GameTime);
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

    movementComponent.TargetLocation = locationComponent.WorldLocation;
    if (bIsHorizontal)
    {
        movementComponent.TargetLocation.X += value;
    }
    else
    {
        movementComponent.TargetLocation.Y += value;
    }

    auto movementSystem = game.GetSystem<MovementSystem>();
    if (movementSystem->IsValidMove(possessedEntity, locationComponent.WorldLocation, movementComponent.TargetLocation))
    {
        game.StartRound();
    }
}
Entity MainGameMode::CreateCreature(Entity target) const
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();

    if (target == InvalidEntity)
    {
        target = level->CreateEntity();
    }

    componentManager.AddComponent<LocationComponent>(target);
    componentManager.AddComponent<CreatureComponent>(target);
    componentManager.AddComponent<TextureRendererComponent>(target);
    componentManager.AddComponent<CollisionComponent>(target);
    componentManager.AddComponent<MovementComponent>(target);

    return target;
}
