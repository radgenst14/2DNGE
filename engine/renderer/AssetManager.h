#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>

class Renderer;

struct AnimationTag
{
    std::string name;
    int fromFrame;
    int toFrame;
    int direction; // 0=forward, 1=backward, 2=pingpong
};

struct SpriteSheetData
{
    int frameCount{1};
    int frameWidth{0};
    int frameHeight{0};
    std::vector<int> frameDurationsMs;
    std::vector<AnimationTag> tags;
};

class AssetManager
{
public:
    AssetManager(Renderer *renderer) : mRenderer(renderer) {};
    ~AssetManager();

    /* Delete copy constructor and assignment operator */
    AssetManager(const AssetManager &) = delete;
    AssetManager &operator=(const AssetManager &) = delete;

    /**
     * @brief Load a texture from a file and store it under the given ID.
     *        If the ID already exists, the existing texture is returned.
     * @param id Unique string key used to retrieve the texture later.
     * @param filePath Path to the image file on disk.
     * @return The loaded SDL_Texture*, or nullptr on failure.
     */
    SDL_Texture *loadTexture(const std::string &id, const std::string &filePath);

    /**
     * @brief Load a texture from an Aseprite (.ase/.aseprite) file.
     *        Single-frame files produce a static texture.
     *        Multi-frame files produce a horizontal sprite sheet with animation metadata.
     * @param id Unique string key used to retrieve the texture later.
     * @param filePath Path to the .aseprite file on disk.
     * @return The loaded SDL_Texture*, or nullptr on failure.
     */
    SDL_Texture *loadAseprite(const std::string &id, const std::string &filePath);

    /**
     * @brief Get sprite sheet metadata for a loaded aseprite texture.
     * @param id The key the texture was loaded under.
     * @return Pointer to SpriteSheetData, or nullptr if not an aseprite texture.
     */
    const SpriteSheetData *getSpriteSheet(const std::string &id) const;

    /**
     * @brief Retrieve a previously loaded texture by its ID.
     * @param id The key the texture was loaded under.
     * @return The SDL_Texture*, or nullptr if not found.
     */
    SDL_Texture *getTexture(const std::string &id) const;

    /**
     * @brief Unload a single texture by its ID, freeing its GPU memory.
     * @param id The key of the texture to remove.
     */
    void unloadTexture(const std::string &id);

    /**
     * @brief Unload all textures, freeing all GPU memory.
     */
    void clear();

    /**
     * @brief Check whether a texture with the given ID is loaded.
     * @param id The key to look up.
     * @return true if the texture exists in the cache.
     */
    bool hasTexture(const std::string &id) const;

    /**
     * @brief Get the width and height of a loaded texture.
     * @param id The key the texture was loaded under.
     * @param width Output parameter for the texture width in pixels.
     * @param height Output parameter for the texture height in pixels.
     * @return true if the texture was found and dimensions were retrieved.
     */
    bool getTextureDimensions(const std::string &id, int &width, int &height) const;

private:
    Renderer *mRenderer;
    std::unordered_map<std::string, SDL_Texture *> mTextures;
    std::unordered_map<std::string, SpriteSheetData> mSpriteSheets;
};

#endif
