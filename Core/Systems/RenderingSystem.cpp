#include "RenderingSystem.h"

#include "Components/LocationComponent.h"
#include "Components/SimpleColorRendererComponent.h"

void RenderingSystem::Execute()
{
    static uint8_t TileSize = 32;

    std::shared_ptr<Renderer> renderer = GetGame()->GetRenderer();
    Query<LocationComponent, SimpleColorRendererComponent>(
        [&renderer](const LocationComponent& location, const SimpleColorRendererComponent& renderData)
        {
            Rect r;
            r.X = location.X * TileSize;
            r.Y = location.Y * TileSize;
            r.Width = TileSize;
            r.Height = TileSize;
            
            renderer->DrawRect(r, "Default");
        });
}
