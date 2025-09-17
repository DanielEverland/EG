// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>
#include <string>

#include "AssetManager.h"
#include "Primitives/Rect.h"

struct SDL_Renderer;

enum class DrawCallOrder : uint8_t
{
    Background,
    Foreground
};

class Renderer
{
    struct DrawCall
    {
        HashedString TextureName = HashedString("Default");
        Rect DestinationRect = Rect(0, 0, 0, 0);
    };
public:
    static Renderer& Get()
    {
        return SingletonHelper::Impl<Renderer>();
    }

    void SetRenderer(SDL_Renderer* rawRenderer)
    {
        assert(rawRenderer);
        SDLRenderer = rawRenderer;
    }

    SDL_Renderer* GetSDLRenderer() const { return SDLRenderer; }

    void Draw(const Rect& rect, const HashedString& textureName, DrawCallOrder order);
    void Present();

private:
    static constexpr size_t MaxNumDrawCalls = 2048;
    static constexpr size_t BackgroundCallsEnd = static_cast<size_t>(MaxNumDrawCalls * (3.0f / 4.0f));

    size_t BackgroundIdx = 0;
    size_t ForegroundIdx = BackgroundCallsEnd + 1;
    
    DrawCall Buffer[sizeof(DrawCall) * MaxNumDrawCalls];
    
    SDL_Renderer* SDLRenderer = nullptr;

    DrawCall* GetDrawCallStruct(DrawCallOrder order);
    void UpdateCameraPosition() const;
};