// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <array>
#include <cassert>
#include <unordered_set>

#include "AssetManager.h"
#include "DataStructrues/Vector.h"
#include "ECS/Entity.h"
#include "GameplayMessages/LocationChangedMessage.h"
#include "Primitives/Color.h"

struct SDL_Renderer;

enum class DrawCallOrder : uint8_t
{
    Background,
    Foreground
};

class Renderer
{
    struct DebugDrawEntry
    {
        virtual ~DebugDrawEntry() = default;
        double EndTime;
    };

    struct DebugDrawBox : DebugDrawEntry
    {
        Rect Rect;
        Color Color;
    };
    
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

    void Draw(const Vector& relativeCameraPosition, const IntVector& destRectSize, const HashedString& textureName, DrawCallOrder order);

    void DrawDebugBox(const Vector& worldPosition, const Vector& size, const Color& color, float duration);

    void AddCameraMovement(const IntVector& delta);
    bool IsWithinWorldSpaceViewport(const Rect& WorldRect) const;
    Rect GetViewportRect() const;
    void SetWorldViewportRect(const Rect& worldRect) { WorldViewportRect = worldRect; }
    size_t GetFrameCount() const { return FrameCount; }
    void Present();
    void Initialize();
    uint8_t GetMaxRenderingDepth() const { return ShadingLevels; }
    std::unordered_set<IntVector> PopDirtyCells();

private:
    static constexpr size_t MaxNumDrawCalls = 8192 * 2 * 2 * 2;
    static constexpr uint8_t StartingShade = 150;
    static constexpr uint8_t ShadingLevels = 8;
    static constexpr uint8_t ShadingPerLevel = (255 - StartingShade) / ShadingLevels;

    Rect WorldViewportRect = { };
    size_t LastDrawcallBufferOverflow = 0;
    size_t FrameCount = 0;

    size_t DrawCallIdx = 0;
    std::array<DrawCall, MaxNumDrawCalls> DrawCalls;
    std::vector<std::unique_ptr<DebugDrawEntry>> DebugDrawEntries;
    std::unordered_set<IntVector> DirtyCells;
    
    SDL_Renderer* SDLRenderer = nullptr;
    SDL_Texture* TerrainRenderTarget = nullptr;
    SDL_Texture* TerrainRenderTargetBackbuffer = nullptr;
    IntVector CameraDelta = IntVector(0, 0, 0);

    DrawCall& GetDrawCallStruct();
    void DrawDebug();
    void OnEntityLocationChanged(const GameplayMessage& baseMsg);
};