// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

#include "Primitives/Rect.h"

struct SDL_Surface;

struct Tileset
{
    SDL_Surface* Surface = nullptr;
    SDL_Texture* Texture = nullptr;
    std::unordered_map<std::string, Rect> SourceRects;

    bool IsValid() const { return Surface != nullptr; }
};

class AssetManager
{
public:
    static AssetManager& Get()
    {
        if (Instance == nullptr)
        {
            Instance = new AssetManager();
        }
        return *Instance;
    }
    
    void Load();
    [[nodiscard]] std::filesystem::path GetPath(const std::string& assetName) const;
    [[nodiscard]] const Tileset& GetTileset() const;

private:
    AssetManager() = default;
    static inline AssetManager* Instance = nullptr;
    
    static const inline std::set<std::string> AssetExtensions = {
        ".json",
        ".bmp"
    };

    std::unordered_map<std::string, std::filesystem::path> DiscoveredAssets;
    Tileset Tileset;
    
    void DiscoverAssets();
    void LoadTileSet();
};
