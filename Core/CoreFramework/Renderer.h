// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <cassert>
#include <string>

#include "AssetManager.h"
#include "Primitives/Rect.h"

struct SDL_Renderer;

class Renderer
{
public:
    static Renderer& Get()
    {
        if (Instance == nullptr)
        {
            Instance = new Renderer(); 
        }
        return *Instance;
    }

    void SetRenderer(SDL_Renderer* rawRenderer)
    {
        assert(rawRenderer);
        SDLRenderer = rawRenderer;
    }

    SDL_Renderer* GetSDLRenderer() const { return SDLRenderer; }

    void DrawRect(const Rect& rect, const std::string& textureName);

private:
    Renderer() = default;
    static inline Renderer* Instance = nullptr;
    
    SDL_Renderer* SDLRenderer = nullptr;
};
