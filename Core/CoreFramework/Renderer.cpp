#include "Renderer.h"

#include <cassert>
#include <iostream>
#include <SDL3/SDL_render.h>
#include <chrono>
#include <SDL3/SDL_hints.h>

#include "Rendering/Texture.h"
#include "Rendering/Tileset.h"
#include "Camera.h"
#include "DebugRenderer.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
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

void Renderer::Draw(const Vector& relativeCameraPosition, const IntVector& destRectSize, const HashedString& textureName, DrawCallOrder order)
{
    DrawCall& drawCall = GetDrawCallStruct();
    drawCall.TextureName = textureName;
    drawCall.RelativeCameraPosition = relativeCameraPosition;
    if (order == DrawCallOrder::Foreground)
        drawCall.RelativeCameraPosition.Z += 0.5f;
    
    drawCall.DestRectSize = destRectSize;
}

void Renderer::Present()
{    
    SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDLRenderer);
    
    Rect viewportRect = GetViewportRect();
    
    SDL_FRect destRect;
    SDL_FRect sourceRect;
    
    std::sort(DrawCalls.begin(), DrawCalls.begin() + DrawCallIdx);
    for (size_t i = 0; i < DrawCallIdx; ++i)
    {
        DrawCall& call = DrawCalls[i];
    
        std::shared_ptr<Texture> texture = AssetManager::Get().GetTexture(call.TextureName);
        assert(texture != nullptr);

        sourceRect = texture->SourceRect;
        destRect.x = call.RelativeCameraPosition.X * static_cast<float>(CellRenderingSize.X) + static_cast<float>(viewportRect.Width) / 2.0f;
        destRect.y = call.RelativeCameraPosition.Y * static_cast<float>(CellRenderingSize.Y) + static_cast<float>(viewportRect.Height) / 2.0f;
        destRect.w = static_cast<float>(call.DestRectSize.X);
        destRect.h = static_cast<float>(call.DestRectSize.Y);

        // -1 since all depths will by definition be below the camera
        float zDepth = std::abs(std::floor(call.RelativeCameraPosition.Z));
        uint8_t truncatedDepth = static_cast<uint8_t>(zDepth);

        constexpr uint8_t StartingShade = 150;
        constexpr uint8_t ShadingLevels = 8;
        constexpr uint8_t ShadingPerLevel = (255 - StartingShade) / ShadingLevels;
        
        if (truncatedDepth > 0)
        {
            uint8_t colorModOffset = std::min(StartingShade + ShadingPerLevel * (truncatedDepth - 1), 255);
            SDL_SetTextureColorMod(texture->Tileset->SDLTexture, 255 - colorModOffset, 255 - colorModOffset, 255 - colorModOffset);
        }
        else
        {
            SDL_SetTextureColorMod(texture->Tileset->SDLTexture, 255, 255, 255);
        }
        SDL_RenderTexture(SDLRenderer, texture->Tileset->SDLTexture, &sourceRect, &destRect);
    }

    DebugRenderer::Draw();
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), SDLRenderer);
    SDL_RenderPresent(SDLRenderer);
    
    DrawCallIdx = 0;
    FrameCount++;
}

Renderer::DrawCall& Renderer::GetDrawCallStruct()
{
    size_t idx = DrawCallIdx++;
    size_t maxPermittedIdx = MaxNumDrawCalls - 1;
    
    size_t nextIdx = idx + 1;
    if (nextIdx > maxPermittedIdx && LastDrawcallBufferOverflow < FrameCount)
    {
        LastDrawcallBufferOverflow = FrameCount;
        std::cerr << "Drawcall buffer overflowed" << std::endl;
    }
    
    // Prevent crash in release
    if (nextIdx > maxPermittedIdx)
    {
        DrawCallIdx--;
    }
    
    return DrawCalls[idx];
}