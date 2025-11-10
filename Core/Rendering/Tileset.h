// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <SDL3/SDL_render.h>

struct Tileset
{
    SDL_Surface* SDLSurface = nullptr;
    SDL_Texture* SDLTexture = nullptr;

    bool IsValid() const { return SDLSurface != nullptr; }
};
