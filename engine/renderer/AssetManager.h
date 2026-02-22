#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#pragma once

#include <string>
#include <unordered_map>
#include <SDL.h>

class Renderer;

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

private:
    Renderer *mRenderer;
    std::unordered_map<std::string, SDL_Texture *> mTextures;
};

#endif
