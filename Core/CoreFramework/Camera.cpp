#include "Camera.h"

IntVector2D Camera::ToViewSpace(const IntVector2D& worldPosition) const
{
    return worldPosition - CameraPosition;
}
