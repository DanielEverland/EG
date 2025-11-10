// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <ostream>
#include <format>
#include <iosfwd>
#include <string>
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

    bool operator==(const Rect& other) const
    {
        return X == other.X && Y == other.Y && Width == other.Width && Height == other.Height;
    }

    bool Intersects(const Rect& other) const;
    bool Intersects(const SDL_FRect& other) const;

    operator SDL_FRect() const;

    std::string ToString() const
    {
        return std::format("({}, {}, {}, {})", X, Y, Width, Height);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Rect& v)
{
    return os << v.ToString();
}