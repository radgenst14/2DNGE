#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#pragma once

#include <string>
#include <SDL.h>

class Renderer;
class AssetManager;

class SpriteRenderer
{
public:
    SpriteRenderer(Renderer *renderer, AssetManager *assetManager) : mRenderer(renderer), mAssetManager(assetManager) {};
    ~SpriteRenderer();

    /* Delete copy constructor and assignment operator */
    SpriteRenderer(const SpriteRenderer &) = delete;
    SpriteRenderer &operator=(const SpriteRenderer &) = delete;

    void drawSprite(const std::string &textureId, int x, int y, int width, int height,
                    double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE,
                    const SDL_Rect *srcRect = nullptr);

private:
    Renderer *mRenderer;         ///< Non-owning pointer to the Renderer, used for drawing sprites.
    AssetManager *mAssetManager; ///< Non-owning pointer to the AssetManager, used for retrieving textures.
};

#endif