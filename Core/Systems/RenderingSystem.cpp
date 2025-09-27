#include "RenderingSystem.h"

#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"

void RenderingSystem::Execute()
{
    static uint8_t TileSize = 32;

    Query<LocationComponent, TextureRendererComponent>(
        [](const LocationComponent& location, const TextureRendererComponent& renderData)
        {
            Renderer::Get().Draw(location.WorldLocation, IntVector2D(TileSize, TileSize), renderData.TextureName, renderData.Order);
        });
}
