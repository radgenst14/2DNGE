#pragma once

#include <glm/glm.hpp>

namespace ECS
{
    struct RigidBody
    {
        glm::vec2 velocity{0.0f, 0.0f}; /**< The velocity of the entity in 2D space */
        float mass{1.0f};               /**< The mass of the entity */
    };
}