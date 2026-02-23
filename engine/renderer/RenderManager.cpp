#include "RenderManager.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "helpers/SpriteRenderer.h"
#include "../ecs/EntityManager.h"
#include "../ecs/components/Sprite.h"
#include "../ecs/components/Transform.h"
#include "../ecs/ComponentTypeID.h"

RenderManager::RenderManager(Window *window, EntityManager *entityManager)
    : mEntityManager(entityManager)
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
    mRenderer->setDrawColor(0, 0, 0, 255); // Set clear color to black
    mRenderer->clear();                    // Clear the screen before rendering
    renderSprites();                       // Render all sprites
    mRenderer->present();                  // Update the screen with the rendered content
}

void RenderManager::renderSprites()
{
    for (EntityID entity : mEntityManager->view<ECS::Sprite, ECS::Transform>())
    {
        const ECS::Sprite &sprite = mEntityManager->getComponent<ECS::Sprite>(entity);
        const ECS::Transform &transform = mEntityManager->getComponent<ECS::Transform>(entity);
        mSpriteRenderer->drawSprite(sprite.textureId, static_cast<int>(transform.position.x),
                                    static_cast<int>(transform.position.y), sprite.width, sprite.height,
                                    transform.rotation);
    }
}