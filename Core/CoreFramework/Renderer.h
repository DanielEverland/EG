// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <array>
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
        Vector RelativeCameraPosition = Vector(0, 0, 0);
        IntVector DestRectSize = IntVector(0, 0, 0);

        friend bool operator<(const DrawCall& a, DrawCall b)
        {
            return a.RelativeCameraPosition.Z < b.RelativeCameraPosition.Z;
        }
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
    void Draw(const Vector& relativeCameraPosition, const IntVector& destRectSize, const HashedString& textureName, DrawCallOrder order);
    void Present();

private:
    static constexpr size_t MaxNumDrawCalls = 8192 * 2 * 2 * 2;

    Rect WorldViewportRect = { };
    size_t LastDrawcallBufferOverflow = 0;
    size_t FrameCount = 0;

    size_t DrawCallIdx = 0;
    std::array<DrawCall, MaxNumDrawCalls> DrawCalls;
    
    SDL_Renderer* SDLRenderer = nullptr;

    Renderer::DrawCall& GetDrawCallStruct();
};