#include "AssetManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <SDL3/SDL_surface.h>

#include "json.hpp"
#include "EngineStatics.h"
#include "Renderer.h"

using namespace nlohmann;
using namespace std::filesystem;

void AssetManager::Load()
{
    DiscoverAssets();
    LoadTileSet();
}

const Tileset& AssetManager::GetTileset() const
{
    assert(Tileset.IsValid());
    return Tileset;
}

void AssetManager::DiscoverAssets()
{
    size_t bootDirPathLen = EngineStatics::BootDir.length();
    for (const auto& element : recursive_directory_iterator(EngineStatics::BootDir))
    {
        if (!element.is_regular_file())
            continue;

        if (!AssetExtensions.contains(element.path().extension().string()))
            continue;

        const path& fullPath = element.path();
        const std::string contentPath = fullPath.string().substr(bootDirPathLen);
        DiscoveredAssets.emplace(contentPath, fullPath);
    }
}

void AssetManager::LoadTileSet()
{
    path tileSetPath = GetPath("TilesetInfo.json");

    path infoPath = GetPath("TilesetInfo.json");
    assert(!infoPath.empty());

    std::ifstream f(infoPath.c_str());
    json info = json::parse(f, nullptr, false);
    assert(!info.is_discarded() && "Couldn't parse tileset");
    auto imagePath = info["SourceFile"];

    const path fullImagePath = GetPath(imagePath);
    assert(!fullImagePath.empty());

    Tileset.Surface = SDL_LoadBMP(fullImagePath.string().c_str());
    assert(Tileset.Surface);
    
    Tileset.Texture = SDL_CreateTextureFromSurface(Renderer::Get().GetSDLRenderer(), Tileset.Surface);
    assert(Tileset.Texture);
    SDL_SetTextureScaleMode(Tileset.Texture, SDL_SCALEMODE_NEAREST);

    auto allTiles = info["Tiles"];
    for (auto element : allTiles)
    {
        std::string name = element["Name"];
        auto rectData = element["Rect"];
        Rect sourceRect(rectData["X"], rectData["Y"], rectData["W"], rectData["H"]);
        
        Tileset.SourceRects.emplace(HashedString(name), sourceRect);
    }
}

path AssetManager::GetPath(const std::string& assetName) const
{
    auto iter = DiscoveredAssets.find(assetName);
    if (iter == DiscoveredAssets.end())
    {
        return { };
    }
    return iter->second;
}
