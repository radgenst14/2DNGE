#include "Camera.h"

Camera::~Camera()
{
}

glm::vec2 Camera::worldToScreen(const glm::vec2 &worldPos) const
{
    glm::vec2 relative = worldPos - mPosition; // offset from camera center
    relative *= mZoom;                         // apply zoom
    relative.x += mViewportWidth / 2.0f;       // center on screen
    relative.y += mViewportHeight / 2.0f;
    return relative;
}

glm::vec2 Camera::screenToWorld(const glm::vec2 &screenPos) const
{
    glm::vec2 relative = screenPos;      // start with screen position
    relative.x -= mViewportWidth / 2.0f; // offset from screen center
    relative.y -= mViewportHeight / 2.0f;
    relative /= mZoom;     // apply inverse zoom
    relative += mPosition; // offset by camera position
    return relative;
}