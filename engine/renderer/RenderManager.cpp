#include "RenderManager.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "helpers/SpriteRenderer.h"

RenderManager::RenderManager(Window *window)
{
    mRenderer = std::make_unique<Renderer>(window);
    mAssetManager = std::make_unique<AssetManager>(mRenderer.get());
    mSpriteRenderer = std::make_unique<SpriteRenderer>(mRenderer.get(), mAssetManager.get());
}

RenderManager::~RenderManager()
{
}

void RenderManager::render()
{
    mRenderer->clear();   // Clear the screen before rendering
    renderSprites();      // Render all sprites
    mRenderer->present(); // Update the screen with the rendered content
}

void RenderManager::renderSprites()
{
}