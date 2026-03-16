#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#pragma once

#include <memory>
#include <SDL.h>
#include "Camera.h"

class Window;
class Renderer;
class SpriteRenderer;
class AssetManager;
class EntityManager;

class RenderManager
{
public:
    RenderManager(Window *window, EntityManager *entityManager);
    ~RenderManager();

    /* Delete  copy constructor and assignment operator */
    RenderManager(const RenderManager &) = delete;
    RenderManager &operator=(const RenderManager &) = delete;

    void render();
    void renderSprites();
    void debugDrawColliders();
    void updateAnimations(float dt);

    void setDrawColliders(bool enabled) { mDrawColliders = enabled; }
    bool getDrawColliders() const { return mDrawColliders; }

    AssetManager *getAssetManager() const { return mAssetManager.get(); }
    Camera &getCamera() { return mCamera; }

private:
    std::unique_ptr<Renderer> mRenderer;             ///< Unique pointer to the Renderer, responsible for all rendering operations.
    std::unique_ptr<SpriteRenderer> mSpriteRenderer; ///< Unique pointer to the SpriteRenderer, responsible for drawing sprites using the Renderer and AssetManager.
    std::unique_ptr<AssetManager> mAssetManager;     ///< Unique pointer to the AssetManager, responsible for loading and managing textures.
    EntityManager *mEntityManager;                   ///< Pointer to the EntityManager, used to access entities and their components for rendering.
    Camera mCamera;                                  ///< The Camera instance used for world-to-screen transformations during rendering.
    bool mDrawColliders{false};                       ///< Whether to draw debug collider outlines.
};

#endif