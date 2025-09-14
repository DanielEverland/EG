#include "MainGameMode.h"

#include <iostream>

#include "Components/LocationComponent.h"
#include "Components/SimpleColorRendererComponent.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Level.h"

void MainGameMode::Initialize()
{
    std::shared_ptr<Level> level = GetGame()->GetLevel();
    ComponentManager& componentManager = level->GetComponentManager();

    for (int x = 0; x < 32; ++x)
    {
        for (int y = 0; y < 32; ++y)
        {
            Entity entityId = level->CreateEntity();
            auto& locationComp = componentManager.AddComponent<LocationComponent>(entityId);
            auto& renderComp = componentManager.AddComponent<SimpleColorRendererComponent>(entityId);

            locationComp.X = x;
            locationComp.Y = y;

            renderComp.RectColor = Color(
                static_cast<uint8_t>(rand()) % 256,
                static_cast<uint8_t>(rand()) % 256,
                static_cast<uint8_t>(rand()) % 256);
        }
    }
}
