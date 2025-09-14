// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <string>

#include "AssetManager.h"
#include "Primitives/Rect.h"

struct SDL_Renderer;

class Renderer
{
public:
    Renderer() = default;
    Renderer(SDL_Renderer* rawRenderer, AssetManager& manager) :
        RawRenderer(rawRenderer), AssetManagerInstance(manager) { }

    void DrawRect(const Rect& rect, const std::string& textureName);

private:
    SDL_Renderer* RawRenderer = nullptr;
    AssetManager& AssetManagerInstance;
};
