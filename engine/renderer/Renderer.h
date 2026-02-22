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
    void present() { SDL_RenderPresent(mRenderer); }

    /**
     * @brief Clear the current rendering target with the drawing color.
     * @return
     */
    void clear() { SDL_RenderClear(mRenderer); }

    /**
     * @brief Set the color used for drawing operations (Rect, Line and Clear).
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { SDL_SetRenderDrawColor(mRenderer, r, g, b, a); }

    /**
     * @brief Draw a outline grid on the screen with the specified cell size and color.
     * @param cellSize
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void drawGrid(int cellSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    /**
     * @brief Draw a texture at a position with rotation and scale
     * @param texture SDL_Texture to draw
     * @param transform Transform component with float position/rotation/scale
     */
    void drawTexture(SDL_Texture *texture, const ECS::Transform &transform);

    /**
     * @brief Draw a filled rectangle
     */
    void drawRect(const ECS::Transform &transform, int width, int height);

    /**
     * @brief Get the underlying SDL_Renderer pointer.
     * @return SDL_Renderer*
     */
    SDL_Renderer *getSDLRenderer() const { return mRenderer; }

private:
    SDL_Renderer *mRenderer = nullptr; /**< The SDL renderer associated with this class */
};

#endif