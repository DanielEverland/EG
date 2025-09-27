#include "MainGameMode.h"
#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"
#include "CoreFramework/Camera.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"
#include "Input/Input.h"
#include "Input/InputAction.h"

void MainGameMode::Initialize()
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();
    
    for (int x = 0; x < 32; ++x)
    {
        for (int y = 0; y < 32; ++y) {
            Entity entityId = level->CreateEntity();
            auto& locationComp = componentManager.AddComponent<LocationComponent>(entityId);
            auto& renderComp = componentManager.AddComponent<TextureRendererComponent>(entityId);

            locationComp.WorldLocation = IntVector2D(x, y);

            renderComp.TextureName = HashedString("Default");
            renderComp.Order = DrawCallOrder::Background;
        }
    }

    Entity playerEntity = level->CreateEntity();
    SetPossessedEntity(playerEntity);
    
    auto& playerLocation = componentManager.AddComponent<LocationComponent>(playerEntity);
    auto& renderComp = componentManager.AddComponent<TextureRendererComponent>(playerEntity);
    
    playerLocation.WorldLocation = IntVector2D(5, 5);

    renderComp.TextureName = HashedString("Player");
    renderComp.Order = DrawCallOrder::Foreground;

    RegisterInput();
}

void MainGameMode::RegisterInput()
{    
    // Horizontal movement
    {
        const auto action = std::make_shared<AxisInputAction>();
        action->SetKeycodeAxis(SDLK_D, 1, InputEventType::DownOrHeld);
        action->SetKeycodeAxis(SDLK_A, -1, InputEventType::DownOrHeld);
        action->SetKeycodeAxis(SDLK_RIGHT, 1, InputEventType::DownOrHeld);
        action->SetKeycodeAxis(SDLK_LEFT, -1, InputEventType::DownOrHeld);
        
        action->AddCallback(std::bind(&MainGameMode::HandleMovementInput, this, std::placeholders::_1, true));

        Input::Get().RegisterAction(action);
    }

    // Vertical movement
    {
        const auto action = std::make_shared<AxisInputAction>();
        action->SetKeycodeAxis(SDLK_W, 1, InputEventType::DownOrHeld);
        action->SetKeycodeAxis(SDLK_S, -1, InputEventType::DownOrHeld);
        action->SetKeycodeAxis(SDLK_UP, 1, InputEventType::DownOrHeld);
        action->SetKeycodeAxis(SDLK_DOWN, -1, InputEventType::DownOrHeld);
        
        action->AddCallback(std::bind(&MainGameMode::HandleMovementInput, this, std::placeholders::_1, false));

        Input::Get().RegisterAction(action);
    }
}
void MainGameMode::HandleMovementInput(int32_t value, bool bIsHorizontal)
{
    Entity possessedEntity = Game::Get().GetGameMode()->GetPossessedEntity();
    if (possessedEntity == InvalidEntity)
        return;

    if (const auto locationComponent = Game::Get().GetLevel()->GetComponentManager().TryGetComponent<LocationComponent>(possessedEntity))
    {
        if (bIsHorizontal)
        {
            locationComponent->WorldLocation.X += value;
        }
        else
        {
            locationComponent->WorldLocation.Y -= value;
        }
    }
}
