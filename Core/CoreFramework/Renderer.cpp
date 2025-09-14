#include "Renderer.h"

#include <SDL3/SDL_render.h>

void Renderer::DrawRect(const Rect& rect, const Color& color)
{
    SDL_FRect sdlRect = rect;
    SDL_SetRenderDrawColor(RawRenderer, color.R, color.G, color.B, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(RawRenderer, &sdlRect);
}
    