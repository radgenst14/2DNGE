#include "RenderManager.h"
#include "Renderer.h"
#include "AssetManager.h"
#include "helpers/SpriteRenderer.h"
#include "../core/ecs/EntityManager.h"
#include "../core/ecs/components/Sprite.h"
#include "../core/ecs/components/Transform.h"
#include "../core/ecs/ComponentTypeID.h"

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

        mSpriteRenderer->drawSprite(sprite.textureId, static_cast<int>(transform.position.x),
                                    static_cast<int>(transform.position.y), sprite.width, sprite.height,
                                    transform.rotation, SDL_FLIP_NONE, srcRect);
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