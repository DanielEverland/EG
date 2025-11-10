#include "RenderingSystem.h"

#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"
#include "CoreFramework/GameMode.h"

void RenderingSystem::Execute()
{
    static IntVector2D tileSize = IntVector2D(16 * 2, 24 * 2);
    Entity possessedEntity = Game::Get().GetGameMode()->GetPossessedEntity();
    auto possessedLocationComp = Game::Get().GetLevel()->GetComponentManager().GetComponentChecked<LocationComponent>(possessedEntity);
    IntVector2D position = possessedLocationComp.WorldLocation;

    Query<LocationComponent, TextureRendererComponent>(
        [position](const LocationComponent& location, const TextureRendererComponent& renderData)
        {
            IntVector2D diff = position - location.WorldLocation;
            if (diff.Distance() > 15)
                return;
            
            Renderer::Get().Draw(location.WorldLocation, tileSize, renderData.TextureName, renderData.Order);
        });
}
