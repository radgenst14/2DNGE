#pragma once

#include <string>

namespace ECS
{
    struct Sprite
    {
        std::string textureId; /**< The ID of the texture to use for this sprite, as defined in the AssetManager. */
        int width{0};          /**< The width of the sprite in pixels. */
        int height{0};         /**< The height of the sprite in pixels. */
    };
}