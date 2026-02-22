#include "AssetManager.h"
#include "Renderer.h"
#include <SDL_image.h>

AssetManager::~AssetManager()
{
    clear();
}

SDL_Texture *AssetManager::loadTexture(const std::string &id, const std::string &filePath)
{
    // Return existing texture if already loaded under this ID
    auto it = mTextures.find(id);
    if (it != mTextures.end())
        return it->second;

    // Load the image into a surface
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        SDL_Log("AssetManager: Failed to load image '%s': %s", filePath.c_str(), IMG_GetError());
        return nullptr;
    }

    // Create a GPU texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer->getSDLRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        SDL_Log("AssetManager: Failed to create texture from '%s': %s", filePath.c_str(), SDL_GetError());
        return nullptr;
    }

    mTextures[id] = texture;
    return texture;
}

SDL_Texture *AssetManager::getTexture(const std::string &id) const
{
    auto it = mTextures.find(id);
    if (it != mTextures.end())
        return it->second;

    return nullptr;
}

void AssetManager::unloadTexture(const std::string &id)
{
    auto it = mTextures.find(id);
    if (it != mTextures.end())
    {
        SDL_DestroyTexture(it->second);
        mTextures.erase(it);
    }
}

void AssetManager::clear()
{
    for (auto &[id, texture] : mTextures)
        SDL_DestroyTexture(texture);

    mTextures.clear();
}

bool AssetManager::hasTexture(const std::string &id) const
{
    return mTextures.find(id) != mTextures.end();
}
