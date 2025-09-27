#include "Renderer.h"

#include <cassert>
#include <SDL3/SDL_render.h>

#include "Camera.h"
#include "Level.h"

void Renderer::Draw(const Vector2D& worldPosition, const IntVector2D& destRectSize, const HashedString& textureName, DrawCallOrder order)
{
    DrawCall* container = GetDrawCallStruct(order);
    container->TextureName = textureName;
    container->WorldPosition = worldPosition;
    container->DestRectSize = destRectSize;
}

void Renderer::Present()
{    
    SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDLRenderer);
    
    const Tileset& tileSet = AssetManager::Get().GetTileset();
    
    SDL_Rect viewportSize;
    SDL_GetRenderViewport(SDLRenderer, &viewportSize);

    Vector2D cameraPos = Camera::Get().GetPosition();
    SDL_FRect destRect;
    SDL_FRect sourceRect;
    DrawCall* call;
    auto DoDraw = [&](size_t idx)
    {
        call = &Buffer[idx];
        
        assert(tileSet.SourceRects.contains(call->TextureName));

        sourceRect = tileSet.SourceRects.at(call->TextureName);
        destRect.x = (call->WorldPosition.X - cameraPos.X) * CellSize_f + static_cast<float>(viewportSize.w) / 2.0f;
        destRect.y = (call->WorldPosition.Y - cameraPos.Y) * CellSize_f + static_cast<float>(viewportSize.h) / 2.0f;
        destRect.w = static_cast<float>(call->DestRectSize.X);
        destRect.h = static_cast<float>(call->DestRectSize.Y);

        // TODO: Any batch render call?
        SDL_RenderTexture(SDLRenderer, tileSet.Texture, &sourceRect, &destRect);
    };

    for (size_t i = 0; i < BackgroundIdx; ++i)
    {
        DoDraw(i);
    }

    for (size_t i = BackgroundCallsEnd + 1; i < ForegroundIdx; ++i)
    {
        DoDraw(i);
    }

    SDL_RenderPresent(SDLRenderer);

    BackgroundIdx = 0;
    ForegroundIdx = BackgroundCallsEnd + 1;
}

Renderer::DrawCall* Renderer::GetDrawCallStruct(DrawCallOrder order)
{
    size_t* curr = (order == DrawCallOrder::Background) ? &BackgroundIdx : &ForegroundIdx;
    size_t max = (order == DrawCallOrder::Background) ? BackgroundCallsEnd : MaxNumDrawCalls;

    size_t idx = *curr;
    
    size_t nextIdx = idx + 1;
    assert(nextIdx <= max);
    
    // Fallback to overwrite last element in release builds
    if (nextIdx <= max)
    {
        *curr = nextIdx;
    }
    
    return &Buffer[idx];
}