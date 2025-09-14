// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include "Primitives/Color.h"
#include "Primitives/Rect.h"

struct SDL_Renderer;

class Renderer
{
public:
    Renderer() = default;
    Renderer(SDL_Renderer* rawRenderer) : RawRenderer(rawRenderer) { }

    void DrawRect(const Rect& rect, const Color& color);

private:
    SDL_Renderer* RawRenderer = nullptr;
};
