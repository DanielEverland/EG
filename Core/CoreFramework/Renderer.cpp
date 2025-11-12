#include "Renderer.h"

#include <cassert>
#include <iostream>
#include <SDL3/SDL_render.h>
#include <chrono>

#include "Rendering/Texture.h"
#include "Rendering/Tileset.h"
#include "Camera.h"
#include "Level.h"

bool Renderer::IsWithinWorldSpaceViewport(const Rect& WorldRect) const
{
    return WorldViewportRect.Intersects(WorldRect);
}

Rect Renderer::GetViewportRect() const
{
    SDL_Rect viewportSize;
    SDL_GetRenderViewport(SDLRenderer, &viewportSize);
    return Rect
    {
        viewportSize.x,
        viewportSize.y,
        viewportSize.w,
        viewportSize.h
    };
}

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
    
    Rect viewportRect = GetViewportRect();
    Vector2D cameraPos = Camera::Get().GetPosition();
    
    SDL_FRect destRect;
    SDL_FRect sourceRect;
    DrawCall* call;
    auto DoDraw = [&](size_t idx)
    {
        call = &Buffer[idx];
    
        std::shared_ptr<Texture> texture = AssetManager::Get().GetTexture(call->TextureName);
        assert(texture != nullptr);

        sourceRect = texture->SourceRect;
        destRect.x = (call->WorldPosition.X - cameraPos.X) * static_cast<float>(CellRenderingSize.X) + static_cast<float>(viewportRect.Width) / 2.0f;
        destRect.y = (call->WorldPosition.Y - cameraPos.Y) * static_cast<float>(CellRenderingSize.Y) + static_cast<float>(viewportRect.Height) / 2.0f;
        destRect.w = static_cast<float>(call->DestRectSize.X);
        destRect.h = static_cast<float>(call->DestRectSize.Y);

        // TODO: Any batch render call?
        SDL_RenderTexture(SDLRenderer, texture->Tileset->SDLTexture, &sourceRect, &destRect);
    };

    //auto begin = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < BackgroundIdx; ++i)
    {
        DoDraw(i);
    }

    for (size_t i = BackgroundCallsEnd + 1; i < ForegroundIdx; ++i)
    {
        DoDraw(i);
    }

    /*auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Render time passed " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[μs]" << '\n';*/
    
    SDL_RenderPresent(SDLRenderer);

    BackgroundIdx = 0;
    ForegroundIdx = BackgroundCallsEnd + 1;
    FrameCount++;
}

Renderer::DrawCall* Renderer::GetDrawCallStruct(DrawCallOrder order)
{
    size_t* curr = (order == DrawCallOrder::Background) ? &BackgroundIdx : &ForegroundIdx;
    size_t max = (order == DrawCallOrder::Background) ? BackgroundCallsEnd : MaxNumDrawCalls;

    size_t idx = *curr;
    
    size_t nextIdx = idx + 1;
    if (nextIdx > max && LastDrawcallBufferOverflow < FrameCount)
    {
        LastDrawcallBufferOverflow = FrameCount;
        std::cerr << "Drawcall buffer overflowed" << std::endl;
    }
    
    // Fallback to overwrite last element
    if (nextIdx <= max)
    {
        *curr = nextIdx;
    }
    
    return &Buffer[idx];
}