// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cstdint>
#include <SDL3/SDL_rect.h>

struct Rect
{
    Rect() = default;
    Rect(int32_t x, int32_t y, int32_t width, int32_t height)
        : X(x), Y(y), Width(width), Height(height) {}

    int32_t X;
    int32_t Y;
    int32_t Width;
    int32_t Height;

    operator SDL_FRect() const;
};
