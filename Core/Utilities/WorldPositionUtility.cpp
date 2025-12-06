#include "WorldPositionUtility.h"

#include <cassert>

#include "CoreFramework/Camera.h"
#include "CoreFramework/Game.h"
#include "CoreFramework/Renderer.h"

Vector2D WorldPositionUtility::ScreenPositionToWorld(const Vector2D& screenPos)
{
    SDL_Rect rect;
    SDL_GetRenderViewport(Renderer::Get().GetSDLRenderer(), &rect);

    Vector2D relativeScreenPosToCenter = screenPos - Vector2D(rect.w / 2.0f, rect.h / 2.0f);
    Vector2D relativeCameraOffsetWorldPos = Vector2D(
        relativeScreenPosToCenter.X / Renderer::CellRenderingSize.X,
        relativeScreenPosToCenter.Y / Renderer::CellRenderingSize.Y);

    IntVector cameraPos = Camera::Get().GetPosition();

    return Vector2D(
        static_cast<float>(cameraPos.X) + relativeCameraOffsetWorldPos.X,
        static_cast<float>(cameraPos.Y) + relativeCameraOffsetWorldPos.Y);
}

IntVector WorldPositionUtility::WorldPositionToCellPosition(const Vector& worldPosition)
{
    return IntVector(static_cast<int32_t>(floor(worldPosition.X)), static_cast<int32_t>(floor(worldPosition.Y)), static_cast<int32_t>(floor(worldPosition.Z)));
}

IntVector WorldPositionUtility::WorldPositionToChunkPosition(IntVector WorldPosition)
{
    // Required to handle negative values
    static auto DoDivFloor = [](int Value, int Size) {
        return Value >= 0 ? Value / Size : (Value - Size + 1) / Size;
    };

    return IntVector
    {
        DoDivFloor(WorldPosition.X, ChunkWidth),
        DoDivFloor(WorldPosition.Y, ChunkHeight),
        WorldPosition.Z
    };
}

IntVector WorldPositionUtility::WorldSpaceToChunkSpace(IntVector WorldPosition)
{
    const IntVector chunkPos = WorldPositionToChunkPosition(WorldPosition);
    return IntVector
    {
        WorldPosition.X - chunkPos.X * ChunkWidth,
        WorldPosition.Y - chunkPos.Y * ChunkHeight,
        WorldPosition.Z - chunkPos.Z
    };
}

IntVector WorldPositionUtility::ChunkSpaceToWorldPosition(const IntVector& ChunkPosition, const IntVector2D& LocalPosition)
{
    return IntVector
    {
        ChunkPosition.X * ChunkWidth + LocalPosition.X,
        ChunkPosition.Y * ChunkHeight + LocalPosition.Y,
        ChunkPosition.Z
    };
}
bool WorldPositionUtility::IsChunkSpacePositionInChunk(const IntVector& pos)
{
    return pos.X >= 0 && pos.Y >= 0 && pos.Z == 0 && pos.X < ChunkWidth && pos.Y < ChunkHeight;
}

int32_t WorldPositionUtility::GetTileIndex(const IntVector& chunkSpacePos)
{
    assert(IsChunkSpacePositionInChunk(chunkSpacePos));
    return chunkSpacePos.X + chunkSpacePos.Y * ChunkWidth;
}
