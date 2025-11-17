#include "RenderingSystem.h"

#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"

void RenderingSystem::Execute()
{
    Renderer& renderer = Renderer::Get();
    static IntVector2D tileSize = IntVector2D(16 * 2, 24 * 2);
    
    Query<LocationComponent, TextureRendererComponent>(
        [&](const LocationComponent& location, const TextureRendererComponent& renderData)
        {
            Rect worldRect = Rect
            {
                location.GetLocation().X * tileSize.X - tileSize.X / 2,
                location.GetLocation().Y * tileSize.Y - tileSize.Y / 2,
                tileSize.X,
                tileSize.Y,
            };

            if (renderer.IsWithinWorldSpaceViewport(worldRect))
            {
                renderer.Draw(location.GetLocation(), tileSize, renderData.TextureName, renderData.Order);
            }
        }
    );
}
