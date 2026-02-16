#pragma once

#include <glm/glm.hpp>

namespace ECS
{
    enum class ColliderType
    {
        Box,
        Circle
    };

    struct Collider
    {
        ColliderType type{ColliderType::Box};

        // For Box collider
        glm::vec2 size{1.0f, 1.0f};   /**< Width and height of the box */
        glm::vec2 offset{0.0f, 0.0f}; /**< Offset from Transform position */

        // For Circle collider
        float radius{0.5f}; /**< Radius for circle collider */

        // Common properties
        bool isTrigger{false}; /**< True if collision doesn't block movement */
    };
}