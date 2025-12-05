#include "Rect.h"

bool Rect::Intersects(const Rect& other) const
{
    return !(   X + Width < other.X
            ||  X > other.X + other.Width
            ||  Y + Height < other.Y
            ||  Y > other.Y + other.Height
            );
}

bool Rect::Intersects(const SDL_FRect& other) const
{
    return Intersects(Rect(static_cast<int32_t>(other.x), static_cast<int32_t>(other.y), static_cast<int32_t>(other.w), static_cast<int32_t>(other.h)));
}

bool Rect::Contains(const IntVector2D point) const
{   
    return point.X >= X && point.X < X + Width && point.Y >= Y && point.Y < Y + Height;
}

Rect::operator SDL_FRect() const
{
    return SDL_FRect(
        static_cast<float>(X),
        static_cast<float>(Y),
        static_cast<float>(Width),
        static_cast<float>(Height));
}
