#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#pragma once

#include <memory>
#include <SDL.h>

class Renderer;
class Window;
class AssetManager;
class SpriteRenderer;

class RenderManager
{
public:
    RenderManager(Window *window);
    ~RenderManager();

    /* Delete  copy constructor and assignment operator */
    RenderManager(const RenderManager &) = delete;
    RenderManager &operator=(const RenderManager &) = delete;

    void render();
    void renderSprites();

private:
    std::unique_ptr<Renderer> mRenderer;             ///< Unique pointer to the Renderer, responsible for all rendering operations.
    std::unique_ptr<AssetManager> mAssetManager;     ///< Unique pointer to the AssetManager, responsible for loading and managing textures.
    std::unique_ptr<SpriteRenderer> mSpriteRenderer; ///< Unique pointer to the SpriteRenderer, responsible for drawing sprites using the Renderer and AssetManager.
};

#endif