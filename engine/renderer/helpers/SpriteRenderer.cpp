#include "SpriteRenderer.h"
#include "../AssetManager.h"
#include "../Renderer.h"

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::drawSprite(const std::string &textureId, int x, int y, int width, int height,
                                double angle, SDL_RendererFlip flip)
{
    SDL_Texture *texture = mAssetManager->getTexture(textureId);
    if (!texture)
    {
        SDL_Log("SpriteRenderer: Texture '%s' not found", textureId.c_str());
        return;
    }

    SDL_Rect dst = {x, y, width, height};
    SDL_RenderCopyEx(mRenderer->getSDLRenderer(), texture, nullptr, &dst, angle, nullptr, flip);
}