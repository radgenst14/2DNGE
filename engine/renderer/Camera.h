#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera() : mViewportWidth(0), mViewportHeight(0) {};
    Camera(int viewportWidth, int viewportHeight) : mViewportWidth(viewportWidth), mViewportHeight(viewportHeight) {};
    ~Camera();

    glm::vec2 getPosition() const { return mPosition; };
    void setPosition(const glm::vec2 &position) { mPosition = position; };
    void move(const glm::vec2 &delta) { mPosition += delta; };

    float getZoom() const { return mZoom; };
    void setZoom(float zoom) { mZoom = zoom; };

    glm::vec2 worldToScreen(const glm::vec2 &worldPos) const;
    glm::vec2 screenToWorld(const glm::vec2 &screenPos) const;

private:
    glm::vec2 mPosition{0.0f, 0.0f}; ///< The current position of the camera in world coordinates.
    int mViewportWidth;              ///< The width of the camera's viewport in pixels.
    int mViewportHeight;             ///< The height of the camera's viewport in pixels.
    float mZoom{1.0f};               ///< The current zoom level of the camera.
};

#endif