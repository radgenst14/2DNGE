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

    SpriteSheetData sheetData;
    sheetData.frameWidth = ase->w;
    sheetData.frameHeight = ase->h;
    sheetData.frameCount = ase->frame_count;

    SDL_Texture *texture = nullptr;
    int pitch = ase->w * (int)sizeof(ase_color_t);

    if (ase->frame_count == 1)
    {
        // Static sprite: single texture, same as a PNG
        texture = SDL_CreateTexture(
            mRenderer->getSDLRenderer(),
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STATIC,
            ase->w, ase->h);

        if (!texture)
        {
            SDL_Log("AssetManager: Failed to create texture from aseprite '%s': %s", filePath.c_str(), SDL_GetError());
            cute_aseprite_free(ase);
            return nullptr;
        }

        SDL_UpdateTexture(texture, nullptr, ase->frames[0].pixels, pitch);
    }
    else
    {
        // Animated sprite: bake all frames into a horizontal sprite sheet
        int sheetWidth = ase->w * ase->frame_count;
        texture = SDL_CreateTexture(
            mRenderer->getSDLRenderer(),
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STATIC,
            sheetWidth, ase->h);

        if (!texture)
        {
            SDL_Log("AssetManager: Failed to create sprite sheet from aseprite '%s': %s", filePath.c_str(), SDL_GetError());
            cute_aseprite_free(ase);
            return nullptr;
        }

        for (int i = 0; i < ase->frame_count; ++i)
        {
            SDL_Rect dst = {i * ase->w, 0, ase->w, ase->h};
            SDL_UpdateTexture(texture, &dst, ase->frames[i].pixels, pitch);
            sheetData.frameDurationsMs.push_back(ase->frames[i].duration_milliseconds);
        }

        // Copy animation tags
        for (int i = 0; i < ase->tag_count; ++i)
        {
            AnimationTag tag;
            tag.name = ase->tags[i].name;
            tag.fromFrame = ase->tags[i].from_frame;
            tag.toFrame = ase->tags[i].to_frame;
            tag.direction = static_cast<int>(ase->tags[i].loop_animation_direction);
            sheetData.tags.push_back(std::move(tag));
        }
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    cute_aseprite_free(ase);

    mTextures[id] = texture;
    mSpriteSheets[id] = std::move(sheetData);
    return texture;
}

SDL_Texture *AssetManager::getTexture(const std::string &id) const
{
    auto it = mTextures.find(id);
    if (it != mTextures.end())
        return it->second;

    return nullptr;
}

const SpriteSheetData *AssetManager::getSpriteSheet(const std::string &id) const
{
    auto it = mSpriteSheets.find(id);
    if (it != mSpriteSheets.end())
        return &it->second;
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
    mSpriteSheets.erase(id);
}

void AssetManager::clear()
{
    for (auto &[id, texture] : mTextures)
        SDL_DestroyTexture(texture);

    mTextures.clear();
    mSpriteSheets.clear();
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
