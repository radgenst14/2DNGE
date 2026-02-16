#ifndef RENDERER_H
#define RENDERER_H

#pragma once

#include <SDL.h>
#include "../ecs/components/Transform.h"

class Window;

class Renderer
{
public:
    Renderer(Window *window);
    ~Renderer();

    /* Delete copy constructor and assignment operator */
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    /**
     * @brief Update the screen with any rendering performed since the previous call.
     * @return
     */
    void Present() { SDL_RenderPresent(mRenderer); }

    /**
     * @brief Clear the current rendering target with the drawing color.
     * @return
     */
    void Clear() { SDL_RenderClear(mRenderer); }

    /**
     * @brief Set the color used for drawing operations (Rect, Line and Clear).
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { SDL_SetRenderDrawColor(mRenderer, r, g, b, a); }

    /**
     * @brief Draw a outline grid on the screen with the specified cell size and color.
     * @param cellSize
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void DrawGrid(int cellSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /**
     * @brief Draw a texture at a position with rotation and scale
     * @param texture SDL_Texture to draw
     * @param transform Transform component with float position/rotation/scale
     */
    void DrawTexture(SDL_Texture *texture, const ECS::Transform &transform);

    /**
     * @brief Draw a filled rectangle
     */
    void DrawRect(const ECS::Transform &transform, int width, int height);

private:
    SDL_Renderer *mRenderer = nullptr; /**< The SDL renderer associated with this class */
};

#endif