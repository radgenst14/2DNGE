#include "RenderManager.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "helpers/SpriteRenderer.h"
#include "../core/Window.h"
#include "../core/ecs/EntityManager.h"
#include "../core/ecs/components/Sprite.h"
#include "../core/ecs/components/Transform.h"
#include "../core/ecs/components/Collider.h"
#include "../core/ecs/ComponentTypeID.h"

RenderManager::RenderManager(Window *window, EntityManager *entityManager)
    : mEntityManager(entityManager),
      mCamera(window->getWidth(), window->getHeight())
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
    if (mDrawColliders)
        debugDrawColliders();              // Draw debug collider outlines
    mRenderer->present();                  // Update the screen with the rendered content
}

void RenderManager::renderSprites()
{
    for (EntityID entity : mEntityManager->view<ECS::Sprite, ECS::Transform>())
    {
        const ECS::Sprite &sprite = mEntityManager->getComponent<ECS::Sprite>(entity);
        const ECS::Transform &transform = mEntityManager->getComponent<ECS::Transform>(entity);

        const SDL_Rect *srcRect = nullptr;
        SDL_Rect frameRect;

        const SpriteSheetData *sheet = mAssetManager->getSpriteSheet(sprite.textureId);
        if (sheet && sheet->frameCount > 1)
        {
            frameRect = {
                sprite.currentFrame * sheet->frameWidth,
                0,
                sheet->frameWidth,
                sheet->frameHeight};
            srcRect = &frameRect;
        }

        glm::vec2 screenPos = mCamera.worldToScreen(transform.position);
        float zoom = mCamera.getZoom();
        int drawW = static_cast<int>(sprite.width * zoom);
        int drawH = static_cast<int>(sprite.height * zoom);
        int drawX = static_cast<int>(screenPos.x) - drawW / 2;
        int drawY = static_cast<int>(screenPos.y) - drawH / 2;
        mSpriteRenderer->drawSprite(sprite.textureId, drawX, drawY, drawW, drawH,
                                    transform.rotation, SDL_FLIP_NONE, srcRect);
    }
}

void RenderManager::debugDrawColliders()
{
    mRenderer->setDrawColor(0, 255, 0, 255); // Green outlines

    for (EntityID entity : mEntityManager->view<ECS::Collider, ECS::Transform>())
    {
        const auto &collider = mEntityManager->getComponent<ECS::Collider>(entity);
        const auto &transform = mEntityManager->getComponent<ECS::Transform>(entity);

        glm::vec2 worldPos = transform.position + collider.offset;
        glm::vec2 screenPos = mCamera.worldToScreen(worldPos);
        float zoom = mCamera.getZoom();

        if (collider.type == ECS::ColliderType::Box)
        {
            int w = static_cast<int>(collider.size.x * zoom);
            int h = static_cast<int>(collider.size.y * zoom);
            int x = static_cast<int>(screenPos.x) - w / 2;
            int y = static_cast<int>(screenPos.y) - h / 2;
            mRenderer->drawRectOutline(x, y, w, h);
        }
        else if (collider.type == ECS::ColliderType::Circle)
        {
            int r = static_cast<int>(collider.radius * zoom);
            mRenderer->drawCircleOutline(
                static_cast<int>(screenPos.x),
                static_cast<int>(screenPos.y),
                r);
        }
    }
}

void RenderManager::updateAnimations(float dt)
{
    for (EntityID entity : mEntityManager->view<ECS::Sprite>())
    {
        ECS::Sprite &sprite = mEntityManager->getComponent<ECS::Sprite>(entity);

        const SpriteSheetData *sheet = mAssetManager->getSpriteSheet(sprite.textureId);
        if (!sheet || sheet->frameCount <= 1 || !sprite.playing)
            continue;

        // Determine active frame range from tag (or use all frames)
        int fromFrame = 0;
        int toFrame = sheet->frameCount - 1;
        int animDirection = 0; // 0=forward

        if (sprite.currentTag >= 0 && sprite.currentTag < static_cast<int>(sheet->tags.size()))
        {
            const AnimationTag &tag = sheet->tags[sprite.currentTag];
            fromFrame = tag.fromFrame;
            toFrame = tag.toFrame;
            animDirection = tag.direction;
        }

        sprite.elapsed += dt;

        float frameDuration = sheet->frameDurationsMs[sprite.currentFrame] / 1000.0f;

        while (sprite.elapsed >= frameDuration && frameDuration > 0.0f)
        {
            sprite.elapsed -= frameDuration;

            if (animDirection == 2) // pingpong
            {
                sprite.currentFrame += sprite.direction;
                if (sprite.currentFrame > toFrame)
                {
                    sprite.direction = -1;
                    sprite.currentFrame = toFrame - 1;
                    if (sprite.currentFrame < fromFrame)
                    {
                        sprite.currentFrame = fromFrame;
                        if (!sprite.looping)
                        {
                            sprite.playing = false;
                            break;
                        }
                    }
                }
                else if (sprite.currentFrame < fromFrame)
                {
                    sprite.direction = 1;
                    sprite.currentFrame = fromFrame + 1;
                    if (sprite.currentFrame > toFrame)
                    {
                        sprite.currentFrame = toFrame;
                        if (!sprite.looping)
                        {
                            sprite.playing = false;
                            break;
                        }
                    }
                }
            }
            else if (animDirection == 1) // backward
            {
                sprite.currentFrame--;
                if (sprite.currentFrame < fromFrame)
                {
                    if (sprite.looping)
                        sprite.currentFrame = toFrame;
                    else
                    {
                        sprite.currentFrame = fromFrame;
                        sprite.playing = false;
                        break;
                    }
                }
            }
            else // forward
            {
                sprite.currentFrame++;
                if (sprite.currentFrame > toFrame)
                {
                    if (sprite.looping)
                        sprite.currentFrame = fromFrame;
                    else
                    {
                        sprite.currentFrame = toFrame;
                        sprite.playing = false;
                        break;
                    }
                }
            }

            frameDuration = sheet->frameDurationsMs[sprite.currentFrame] / 1000.0f;
        }
    }
}