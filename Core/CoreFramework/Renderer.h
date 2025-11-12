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

    static inline IntVector2D CellSize = IntVector2D(16, 24);
    static constexpr int32_t RenderScale = 2;
    static inline IntVector2D CellRenderingSize = IntVector2D(CellSize.X * RenderScale, CellSize.Y * RenderScale);

    void SetRenderer(SDL_Renderer* rawRenderer)
    {
        assert(rawRenderer);
        SDLRenderer = rawRenderer;
    }

    SDL_Renderer* GetSDLRenderer() const { return SDLRenderer; }

    bool IsWithinWorldSpaceViewport(const Rect& WorldRect) const;
    Rect GetViewportRect() const;
    void SetWorldViewportRect(const Rect& worldRect) { WorldViewportRect = worldRect; }
    size_t GetFrameCount() const { return FrameCount; }
    void Draw(const Vector2D& worldPosition, const IntVector2D& destRectSize, const HashedString& textureName, DrawCallOrder order);
    void Present();

private:
    static constexpr size_t MaxNumDrawCalls = 8192 * 2 * 2 * 2;
    static constexpr size_t BackgroundCallsEnd = static_cast<size_t>(MaxNumDrawCalls * (3.0f / 4.0f));

    Rect WorldViewportRect = { };
    size_t LastDrawcallBufferOverflow = 0;
    size_t FrameCount = 0;
    size_t BackgroundIdx = 0;
    size_t ForegroundIdx = BackgroundCallsEnd + 1;
    
    DrawCall Buffer[MaxNumDrawCalls];
    
    SDL_Renderer* SDLRenderer = nullptr;

    DrawCall* GetDrawCallStruct(DrawCallOrder order);
};