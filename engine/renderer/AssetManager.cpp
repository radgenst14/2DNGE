#include "AssetManager.h"
#include "Renderer.h"
#include <SDL_image.h>
#include "../vendor/cute_aseprite.h"

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

SDL_Texture *AssetManager::loadAseprite(const std::string &id, const std::string &filePath)
{
    // Return existing texture if already loaded under this ID
    auto it = mTextures.find(id);
    if (it != mTextures.end())
        return it->second;

    // Parse the .aseprite file
    ase_t *ase = cute_aseprite_load_from_file(filePath.c_str(), NULL);
    if (!ase)
    {
        SDL_Log("AssetManager: Failed to load aseprite file '%s'", filePath.c_str());
        return nullptr;
    }

    // Create a texture from the first frame's pixel data (RGBA)
    SDL_Texture *texture = SDL_CreateTexture(
        mRenderer->getSDLRenderer(),
        SDL_PIXELFORMAT_ABGR8888, // ase_color_t is {r,g,b,a} = ABGR in SDL byte order
        SDL_TEXTUREACCESS_STATIC,
        ase->w, ase->h);

    if (!texture)
    {
        SDL_Log("AssetManager: Failed to create texture from aseprite '%s': %s", filePath.c_str(), SDL_GetError());
        cute_aseprite_free(ase);
        return nullptr;
    }

    // Upload the first frame's pixels
    SDL_UpdateTexture(texture, nullptr, ase->frames[0].pixels, ase->w * (int)sizeof(ase_color_t));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    cute_aseprite_free(ase);

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

bool AssetManager::getTextureDimensions(const std::string &id, int &width, int &height) const
{
    auto it = mTextures.find(id);
    if (it == mTextures.end())
        return false;

    SDL_QueryTexture(it->second, nullptr, nullptr, &width, &height);
    return true;
}
