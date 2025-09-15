#include "MainGameMode.h"

#include <iostream>

#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"

void MainGameMode::Initialize()
{
    std::shared_ptr<Level> level = GetGame()->GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();
    
    
    for (int x = 0; x < 32; ++x)
    {
        for (int y = 0; y < 32; ++y) {
            Entity entityId = level->CreateEntity();
            auto& locationComp = componentManager.AddComponent<LocationComponent>(entityId);
            auto& renderComp = componentManager.AddComponent<TextureRendererComponent>(entityId);

            locationComp.X = x;
            locationComp.Y = y;

            renderComp.TextureName = HashedString("Default");
            renderComp.Order = DrawCallOrder::Background;
        }
    }

    Entity playerEntity = level->CreateEntity();
    auto& playerLocation = componentManager.AddComponent<LocationComponent>(playerEntity);
    auto& renderComp = componentManager.AddComponent<TextureRendererComponent>(playerEntity);
    
    playerLocation.X = 5;
    playerLocation.Y = 5;

    renderComp.TextureName = HashedString("Player");
    renderComp.Order = DrawCallOrder::Foreground;
}
