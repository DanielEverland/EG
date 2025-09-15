#include "Renderer.h"

#include <cassert>
#include <string>
#include <SDL3/SDL_render.h>

void Renderer::DrawRect(const Rect& rect, const std::string& textureName)
{
    const Tileset& tileSet = AssetManager::Get().GetTileset();
    assert(tileSet.SourceRects.contains(textureName));
    
    SDL_FRect sourceRect = tileSet.SourceRects.at(textureName);    
    SDL_FRect sdlRect = rect;

    SDL_RenderTexture(SDLRenderer, tileSet.Texture, &sourceRect, &sdlRect);
}
    