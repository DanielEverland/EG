// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

#include "DataStructrues/HashedString.h"
#include "Primitives/Rect.h"
#include "Rendering/Texture.h"
#include "Rendering/Tileset.h"
#include "Utilities/SingletonHelpers.h"

struct SDL_Surface;

class AssetManager
{
public:
    static AssetManager& Get()
    {
        return SingletonHelper::Impl<AssetManager>();
    }
    
    void Load();
    bool TryRegisterTexture(HashedString textureName, std::shared_ptr<Texture> textureData);
    
    [[nodiscard]] std::filesystem::path GetPath(const std::string& assetName) const;
    [[nodiscard]] std::vector<std::filesystem::path> GetAssets(const std::filesystem::path& directory) const;
    [[nodiscard]] std::shared_ptr<Texture> GetTexture(HashedString assetName) const;
    [[nodiscard]] std::shared_ptr<Tileset> LoadTileset(const std::string& assetFilePath) const;

private:    
    static const inline std::set<std::string> AssetExtensions = {
        ".json",
        ".bmp"
    };

    std::unordered_map<HashedString, std::shared_ptr<Texture>> TextureLookup;
    std::unordered_map<std::string, std::filesystem::path> DiscoveredAssets;
    
    void DiscoverAssets();
    void LoadTileSet();
};
