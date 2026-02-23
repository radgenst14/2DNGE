#pragma once

#include <string>

namespace ECS
{
    struct Sprite
    {
        std::string textureId; /**< The ID of the texture to use for this sprite, as defined in the AssetManager. */
        int width{0};          /**< The width of the sprite in pixels. */
        int height{0};         /**< The height of the sprite in pixels. */

        // Animation state (engine-driven, ignored for static sprites)
        int currentFrame{0};   /**< Current frame index in the sprite sheet. */
        float elapsed{0.0f};   /**< Seconds accumulated on the current frame. */
        int currentTag{-1};    /**< Index into SpriteSheetData::tags, -1 = play all frames. */
        bool playing{false};   /**< Whether the animation is currently playing. */
        bool looping{true};    /**< Whether the animation loops. */
        int direction{1};      /**< +1 or -1, used for pingpong playback. */
    };
}