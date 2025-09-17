#include "RenderingSystem.h"

#include "Components/LocationComponent.h"
#include "Components/TextureRendererComponent.h"
#include "CoreFramework/Camera.h"

void RenderingSystem::Execute()
{
    static uint8_t TileSize = 32;

    Query<LocationComponent, TextureRendererComponent>(
        [](const LocationComponent& location, const TextureRendererComponent& renderData)
        {
            IntVector2D ViewSpaceLocation = Camera::Get().ToViewSpace(location.WorldLocation);
            
            Rect r;
            r.X = ViewSpaceLocation.X * TileSize;
            r.Y = ViewSpaceLocation.Y * TileSize;
            r.Width = TileSize;
            r.Height = TileSize;
            
            Renderer::Get().Draw(r, renderData.TextureName, renderData.Order);
        });
}
