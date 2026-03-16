#include "Renderer.h"
#include "../core/Window.h"
#include <cmath>

Renderer::Renderer(Window *window)
{
    // Get the SDL_Window from our Window wrapper
    SDL_Window *sdlWindow = window->getWindow();

    // Create the renderer
    mRenderer = SDL_CreateRenderer(
        sdlWindow,                                           // Which window
        -1,                                                  // Auto-select driver
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Hardware + VSync
    );
};

Renderer::~Renderer()
{
    SDL_DestroyRenderer(mRenderer);
}

void Renderer::drawGrid(int cellSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Set the draw color for the grid lines
    setDrawColor(r, g, b, a);

    // Get the size of the rendering target
    int width, height;
    SDL_GetRendererOutputSize(mRenderer, &width, &height);

    // Draw vertical grid lines
    for (int x = 0; x < width; x += cellSize)
    {
        SDL_RenderDrawLine(mRenderer, x, 0, x, height);
    }

    // Draw horizontal grid lines
    for (int y = 0; y < height; y += cellSize)
    {
        SDL_RenderDrawLine(mRenderer, 0, y, width, y);
    }
}

void Renderer::drawTexture(SDL_Texture *texture, const ECS::Transform &transform)
{
    // Query the texture to get its width and height
    int texWidth, texHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

    // Calculate the destination rectangle based on the transform
    SDL_Rect destRect;
    destRect.x = static_cast<int>(transform.position.x - (texWidth * transform.scale.x) / 2);
    destRect.y = static_cast<int>(transform.position.y - (texHeight * transform.scale.y) / 2);
    destRect.w = static_cast<int>(texWidth * transform.scale.x);
    destRect.h = static_cast<int>(texHeight * transform.scale.y);

    // Render the texture with rotation
    SDL_RenderCopyEx(mRenderer, texture, nullptr, &destRect,
                     transform.rotation, nullptr, SDL_FLIP_NONE);
}

void Renderer::drawRect(const ECS::Transform &transform, int width, int height)
{
    // Calculate the destination rectangle based on the transform
    SDL_Rect destRect;
    destRect.x = static_cast<int>(transform.position.x - (width * transform.scale.x) / 2);
    destRect.y = static_cast<int>(transform.position.y - (height * transform.scale.y) / 2);
    destRect.w = static_cast<int>(width * transform.scale.x);
    destRect.h = static_cast<int>(height * transform.scale.y);

    // Render the filled rectangle
    SDL_RenderFillRect(mRenderer, &destRect);
}

void Renderer::drawRectOutline(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(mRenderer, &rect);
}

void Renderer::drawCircleOutline(int cx, int cy, int radius, int segments)
{
    const float step = 2.0f * static_cast<float>(M_PI) / segments;
    for (int i = 0; i < segments; ++i)
    {
        float angle1 = i * step;
        float angle2 = (i + 1) * step;
        int x1 = cx + static_cast<int>(std::cos(angle1) * radius);
        int y1 = cy + static_cast<int>(std::sin(angle1) * radius);
        int x2 = cx + static_cast<int>(std::cos(angle2) * radius);
        int y2 = cy + static_cast<int>(std::sin(angle2) * radius);
        SDL_RenderDrawLine(mRenderer, x1, y1, x2, y2);
    }
}