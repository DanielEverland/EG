#include "RenderingSystem.h"

#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"

void RenderingSystem::Execute()
{
    static uint8_t TileSize = 32;

    Query<LocationComponent, TextureRendererComponent>(
        [](const LocationComponent& location, const TextureRendererComponent& renderData)
        {
            Rect r;
            r.X = location.X * TileSize;
            r.Y = location.Y * TileSize;
            r.Width = TileSize;
            r.Height = TileSize;
            
            Renderer::Get().DrawRect(r, renderData.TextureName);
        });
}
