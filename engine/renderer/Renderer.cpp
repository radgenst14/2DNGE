#include "Renderer.h"
#include "../core/Window.h"

Renderer::Renderer(Window *window)
{
    // Get the SDL_Window from our Window wrapper
    SDL_Window *sdlWindow = window->GetWindow();

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

void Renderer::DrawGrid(int cellSize, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Set the draw color for the grid lines
    SetDrawColor(r, g, b, a);

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

void Renderer::DrawTexture(SDL_Texture *texture, const ECS::Transform &transform)
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

void Renderer::DrawRect(const ECS::Transform &transform, int width, int height)
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