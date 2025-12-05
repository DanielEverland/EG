#include "Renderer.h"

#include <cassert>
#include <iostream>
#include <SDL3/SDL_render.h>
#include <chrono>
#include <unordered_set>
#include <SDL3/SDL_hints.h>

#include "Rendering/Texture.h"
#include "Rendering/Tileset.h"
#include "Camera.h"
#include "DebugRenderer.h"
#include "Game.h"
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

void Renderer::Initialize()
{
    SDL_Rect viewportSize;
    SDL_GetRenderViewport(SDLRenderer, &viewportSize);
    TerrainRenderTarget = SDL_CreateTexture(SDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, viewportSize.w, viewportSize.h);
    TerrainRenderTargetBackbuffer = SDL_CreateTexture(SDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, viewportSize.w, viewportSize.h);

    GameplayMessages::Get().SubscribeMessage(HashedString("EntityLocationChanged"), std::bind(&Renderer::OnEntityLocationChanged, this, std::placeholders::_1));
}

std::unordered_set<IntVector> Renderer::PopDirtyCells()
{
    std::unordered_set<IntVector> temp = DirtyCells;
    DirtyCells.clear();
    return temp;
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

void Renderer::DrawDebugBox(const Vector& worldPosition, const Vector& size, const Color& color, float duration)
{
    DebugDrawBox Entry;

    Entry.Rect.X = static_cast<int32_t>(worldPosition.X); Entry.Rect.Y = static_cast<int32_t>(worldPosition.Y);
    Entry.Rect.Width = static_cast<int32_t>(size.X); Entry.Rect.Height = static_cast<int32_t>(size.Y);
    Entry.EndTime = Game::Get().GetTime() + duration;
    Entry.Color = color;
    
    DebugDrawEntries.push_back(std::make_unique<DebugDrawBox>(Entry));
}

void Renderer::AddCameraMovement(const IntVector& delta)
{
    CameraDelta = CameraDelta + delta;
}

void Renderer::Present()
{    
    SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDLRenderer);
    
    Rect viewportRect = GetViewportRect();
    SDL_FRect sdlViewportRect = viewportRect;
    
    SDL_FRect destRect;
    SDL_FRect sourceRect;

    
    if (CameraDelta.SizeF() > FLT_EPSILON)
    {
        SDL_SetRenderTarget(SDLRenderer, TerrainRenderTargetBackbuffer);
        SDL_FRect moveTargetRect = sdlViewportRect;
        moveTargetRect.x -= CameraDelta.X * CellRenderingSize.X;
        moveTargetRect.y -= CameraDelta.Y * CellRenderingSize.Y;

        SDL_RenderTexture(SDLRenderer, TerrainRenderTarget, &sdlViewportRect, &moveTargetRect);
        CameraDelta = IntVector(0, 0, 0);

        SDL_SetRenderTarget(SDLRenderer, TerrainRenderTarget);
        SDL_RenderTexture(SDLRenderer, TerrainRenderTargetBackbuffer, &sdlViewportRect, &sdlViewportRect);
    }
    
    SDL_SetRenderTarget(SDLRenderer, TerrainRenderTarget);
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
    SDL_SetRenderTarget(SDLRenderer, nullptr);
    SDL_RenderTexture(SDLRenderer, TerrainRenderTarget, &sdlViewportRect, &sdlViewportRect);

    DrawDebug();

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

void Renderer::DrawDebug()
{
    Vector camPos = Camera::Get().GetPosition();
    camPos.X *= static_cast<float>(CellRenderingSize.X);
    camPos.Y *= static_cast<float>(CellRenderingSize.Y);

    Rect viewport = GetViewportRect();
    
    double currTime = Game::Get().GetTime();
    uint8_t oldR, oldG, oldB, oldA;
    SDL_GetRenderDrawColor(SDLRenderer, &oldR, &oldG, &oldB, &oldA);
    for (size_t i = DebugDrawEntries.size() - 1; i > 0; --i)
    {
        if (currTime >= DebugDrawEntries[i]->EndTime)
        {
            DebugDrawEntries.erase(DebugDrawEntries.begin() + i);
        }
        else
        {
            if (const DebugDrawBox* box = dynamic_cast<const DebugDrawBox*>(DebugDrawEntries[i].get()))
            {
                SDL_FRect fRect = box->Rect;
                fRect.x += -camPos.X + viewport.Width / 2.0f;
                fRect.y += -camPos.Y + viewport.Height / 2.0f;
                SDL_SetRenderDrawColor(SDLRenderer, box->Color.R, box->Color.G, box->Color.B, box->Color.A);
                SDL_RenderRect(SDLRenderer, &fRect);
            }
        }
    }
    SDL_SetRenderDrawColor(SDLRenderer, oldR, oldG, oldB, oldA);
}

void Renderer::OnEntityLocationChanged(const GameplayMessage& baseMsg)
{
    auto message = dynamic_cast<const LocationChangedMessage*>(&baseMsg);
    if (message == nullptr)
        return;

    DirtyCells.insert(message->OldLocation);
    DirtyCells.insert(message->NewLocation);
}