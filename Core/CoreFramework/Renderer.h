// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>
#include "AssetManager.h"
#include "DataStructrues/Vector.h"

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
        Vector2D WorldPosition = Vector2D(0, 0);
        IntVector2D DestRectSize = IntVector2D(0, 0);
    };
public:
    static Renderer& Get()
    {
        return SingletonHelper::Impl<Renderer>();
    }

    static constexpr int32_t CellSize = 32;
    static constexpr float CellSize_f = static_cast<float>(CellSize);

    void SetRenderer(SDL_Renderer* rawRenderer)
    {
        assert(rawRenderer);
        SDLRenderer = rawRenderer;
    }

    SDL_Renderer* GetSDLRenderer() const { return SDLRenderer; }

    void Draw(const Vector2D& worldPosition, const IntVector2D& destRectSize, const HashedString& textureName, DrawCallOrder order);
    void Present();

private:
    static constexpr size_t MaxNumDrawCalls = 2048;
    static constexpr size_t BackgroundCallsEnd = static_cast<size_t>(MaxNumDrawCalls * (3.0f / 4.0f));

    size_t BackgroundIdx = 0;
    size_t ForegroundIdx = BackgroundCallsEnd + 1;
    
    DrawCall Buffer[sizeof(DrawCall) * MaxNumDrawCalls];
    
    SDL_Renderer* SDLRenderer = nullptr;

    DrawCall* GetDrawCallStruct(DrawCallOrder order);
};