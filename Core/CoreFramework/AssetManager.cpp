#include "AssetManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "json.hpp"
#include "EngineStatics.h"
#include "Renderer.h"
#include "Utilities/DirectoryHelpers.h"

using namespace nlohmann;
using namespace std::filesystem;

void AssetManager::Load()
{
    DiscoverAssets();
    LoadTileSet();
}

bool AssetManager::TryRegisterTexture(HashedString textureName, std::shared_ptr<Texture> textureData)
{
    if (TextureLookup.contains(textureName))
    {
        std::cerr << "Failed to register texture with name " << textureName.ToString() << ", as it already exists" << std::endl;
        return false;
    }
    TextureLookup.emplace(textureName, textureData);
    return true;
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

    std::shared_ptr<Tileset> tileset = LoadTileset(fullImagePath.string());
    if (tileset == nullptr)
        return;

    auto allTiles = info["Tiles"];
    for (auto element : allTiles)
    {
        std::shared_ptr<Texture> texture = std::make_shared<Texture>();
        
        std::string name = element["Name"];
        auto rectData = element["Rect"];
        Rect sourceRect(rectData["X"], rectData["Y"], rectData["W"], rectData["H"]);

        texture->SourceRect = sourceRect;
        texture->Tileset = tileset;

        // TODO: Report if this fails somewhere central
        TryRegisterTexture(HashedString(name), texture);
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

std::vector<path> AssetManager::GetAssets(const path& directory) const
{
    std::vector<path> toReturn;
    for (const std::pair<const std::string, path>& KVP : DiscoveredAssets)
    {
        if (DirectoryHelpers::IsSubPath(KVP.first, directory))
        {
            toReturn.push_back(KVP.second);
        }
    }
    return toReturn;
}

std::shared_ptr<Texture> AssetManager::GetTexture(HashedString assetName) const
{
    auto iter = TextureLookup.find(assetName);
    if (iter == TextureLookup.end())
        return nullptr;

    return iter->second;
}

std::shared_ptr<Tileset> AssetManager::LoadTileset(const std::string& assetFilePath) const
{
    path path = assetFilePath;
    auto toReturn = std::make_shared<Tileset>();

    if (path.extension() == ".png")
    {
        toReturn->SDLSurface = IMG_Load(assetFilePath.c_str());
        assert(toReturn->SDLSurface);
    }
    else if (path.extension() == ".bmp")
    {
        toReturn->SDLSurface = SDL_LoadBMP(assetFilePath.c_str());
        assert(toReturn->SDLSurface);
    }
    else
    {
        assert(false);
    }    
    
    toReturn->SDLTexture = SDL_CreateTextureFromSurface(Renderer::Get().GetSDLRenderer(), toReturn->SDLSurface);
    assert(toReturn->SDLTexture);
    if (toReturn->SDLTexture == nullptr)
        return nullptr;
    SDL_SetTextureScaleMode(toReturn->SDLTexture, SDL_SCALEMODE_NEAREST);
    return toReturn;
}