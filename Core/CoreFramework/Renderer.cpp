#include "Renderer.h"

#include <cassert>
#include <SDL3/SDL_render.h>

#include "Camera.h"
#include "Game.h"
#include "GameMode.h"
#include "Level.h"
#include "Components/LocationComponent.h"
#include "ECS/Entity.h"

void Renderer::Draw(const Rect& rect, const HashedString& textureName, DrawCallOrder order)
{
    DrawCall* container = GetDrawCallStruct(order);
    container->TextureName = textureName;
    container->DestinationRect = rect;
}
void Renderer::Present()
{
    UpdateCameraPosition();
    
    SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(SDLRenderer);
    
    const Tileset& tileSet = AssetManager::Get().GetTileset();

    SDL_Rect viewportSize;
    SDL_GetRenderViewport(SDLRenderer, &viewportSize);
    
    SDL_FRect destRect;
    SDL_FRect sourceRect;
    DrawCall* call;
    auto DoDraw = [&](size_t idx)
    {
        call = &Buffer[idx];
        
        assert(tileSet.SourceRects.contains(call->TextureName));

        sourceRect = tileSet.SourceRects.at(call->TextureName);
        destRect = call->DestinationRect;
        destRect.x += static_cast<float>(viewportSize.w) / 2.0f;
        destRect.y += static_cast<float>(viewportSize.h) / 2.0f;

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

void Renderer::UpdateCameraPosition() const
{
    Game& game = Game::Get();
    Entity possessedEntity = game.GetGameMode()->GetPossessedEntity();

    // No possessed entity may be valid. Camera should remain in place if so.
    if (possessedEntity == InvalidEntity)
        return;

    ComponentManager& CompManager = game.GetLevel()->GetComponentManager();
    auto Location = CompManager.TryGetComponent<LocationComponent>(possessedEntity);

    // TODO: Is this actually valid? Or should we assert?
    if (Location == nullptr)
        return;

    Camera::Get().GetPosition() = Location->WorldLocation;
}
