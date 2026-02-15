#pragma once

#include <glm/glm.hpp>

namespace ECS
{
    struct Transform
    {
        glm::vec2 position{0.0f, 0.0f}; /**< The position of the entity in 2D space */
        float rotation{0.0f};           /**< The rotation of the entity in degrees */
        glm::vec2 scale{1.0f, 1.0f};    /**< The scale of the entity in 2D space */
    };
}
