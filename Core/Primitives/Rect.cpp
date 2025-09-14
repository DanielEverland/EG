#include "Rect.h"

Rect::operator SDL_FRect() const
{
    return SDL_FRect(
        static_cast<float>(X),
        static_cast<float>(Y),
        static_cast<float>(Width),
        static_cast<float>(Height));
}
