#include "RenderingSystem.h"

#include <unordered_set>

#include "Components/TextureRendererComponent.h"
#include "CoreFramework/Camera.h"
#include "CoreFramework/GameSettings.h"

void RenderingSystem::Execute()
{
    constexpr int32_t renderingMargin = 0;
    
    Renderer& renderer = Renderer::Get();
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    IntVector camLoc = Camera::Get().GetPosition();
    Rect worldViewport = renderer.GetViewportRect();
    worldViewport.Width /= Renderer::CellRenderingSize.X;
    worldViewport.Height /= Renderer::CellRenderingSize.Y;
    worldViewport.Width += renderingMargin * 2;
    worldViewport.Height += renderingMargin * 2;
    worldViewport.X = camLoc.X - worldViewport.Width / 2 - renderingMargin;
    worldViewport.Y = camLoc.Y - worldViewport.Height / 2 - renderingMargin;

    for (int x = worldViewport.X; x < worldViewport.X + worldViewport.Width; ++x)
    {
        for (int y = worldViewport.Y; y < worldViewport.Y + worldViewport.Height; ++y)
        {
            IntVector2D pos = IntVector2D(x, y);
            if (camLoc.Z != lastZDepth || !lastRenderedWorldRect.Contains(pos))
            {
                for (int z = 0; z < renderer.GetMaxRenderingDepth(); ++z)
                {
                    IntVector worldPos = IntVector(pos.X, pos.Y, camLoc.Z - z);
                    DrawEntitiesInCell(worldPos);
                }
            }
        }
    }

    std::unordered_set<IntVector> dirtyCells = renderer.PopDirtyCells();
    for (const IntVector& pos : dirtyCells)
    {
        DrawEntitiesInCell(pos);
    }
    
    lastRenderedWorldRect = worldViewport;
    lastZDepth = camLoc.Z;
}

void RenderingSystem::DrawEntitiesInCell(const IntVector& worldPosition)
{
    std::shared_ptr<Level> level = Game::Get().GetLevel();
    Renderer& renderer = Renderer::Get();
    const EntityContainer& container = level->GetEntitiesAtPosition(worldPosition);
    ComponentManager& componentManager = level->GetComponentManager();

    for (Entity entity : container.Entities)
    {
        if (TextureRendererComponent* renderingComp = componentManager.TryGetComponent<TextureRendererComponent>(entity))
        {
            Vector relativePosition = worldPosition - Camera::Get().GetPosition();
            renderer.Draw(relativePosition, Renderer::CellRenderingSize, renderingComp->TextureName, renderingComp->Order);
        }
    }

    if (GameSettings::Get().DebugDrawRenderedCells)
    {
        // Draw debug rects when updating terrain render target
        Vector pos = Vector(
            static_cast<float>(worldPosition.X * Renderer::CellRenderingSize.X),
            static_cast<float>(worldPosition.Y * Renderer::CellRenderingSize.Y),
            static_cast<float>(worldPosition.Z));
    
        Vector size = Vector(Renderer::CellRenderingSize);
        renderer.DrawDebugBox(pos, size, Color(255, 0, 0, 150), 1.0f);
    }
}
