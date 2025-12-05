#include "Camera.h"

#include "Renderer.h"
#include "Primitives/Rect.h"

void Camera::SetPosition(const IntVector& position)
{
    IntVector delta = position - CameraPosition;
    Renderer::Get().AddCameraMovement(delta);
    
    CameraPosition = position;

    Rect viewportRect = Renderer::Get().GetViewportRect();
    Rect newWorldViewport = Rect
    {
        CameraPosition.X * Renderer::CellRenderingSize.X - viewportRect.Width / 2,
        CameraPosition.Y * Renderer::CellRenderingSize.Y - viewportRect.Height / 2,
        viewportRect.Width,
        viewportRect.Height
    };

    Renderer::Get().SetWorldViewportRect(newWorldViewport);
}

IntVector Camera::ToViewSpace(const IntVector& worldPosition) const
{
    return worldPosition - CameraPosition;
}
