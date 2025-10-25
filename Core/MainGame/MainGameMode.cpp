#include "MainGameMode.h"

#include "Application.h"
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
#include "Systems/CreatureAISystem.h"
#include "Systems/MovementSystem.h"

void MainGameMode::Initialize()
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();

    for (int x = 0; x < 32; ++x)
    {
        for (int y = 0; y < 32; ++y)
        {
            const bool isWall = x == 0 || y == 0 || x == 31 || y == 31;
            const std::string entityType = isWall ? "Wall" : "Floor"; 
            Entity entityId = level->CreateEntity();
            Application::Get().GetEntityFactory()->PopulateEntity(entityId, entityType, IntVector2D(x, y));
        }
    }

    Entity playerEntity = CreateCreature();
    SetPossessedEntity(playerEntity);
    
    auto& playerLocation = componentManager.GetComponentChecked<LocationComponent>(playerEntity);
    auto& renderComp = componentManager.GetComponentChecked<TextureRendererComponent>(playerEntity);
    
    playerLocation.WorldLocation = IntVector2D(5, 5);

    renderComp.TextureName = HashedString("Player");
    renderComp.Order = DrawCallOrder::Foreground;


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

        enemyLocation.WorldLocation = enemyPosition;

        enemyRenderComp.TextureName = HashedString("Enemy");
        enemyRenderComp.Order = DrawCallOrder::Foreground;
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
