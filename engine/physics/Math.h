#ifndef MATH_H
#define MATH_H

#pragma once

#include <cmath>
#include <glm/glm.hpp>

namespace Math
{
    struct AABB
    {
        glm::vec2 min;
        glm::vec2 max;
    };

    inline bool checkAABBCollision(const AABB &aabbA, const AABB &aabbB)
    {
        // Check if the AABBs overlap on both axes
        return (aabbA.min.x < aabbB.max.x && aabbA.max.x > aabbB.min.x &&
                aabbA.min.y < aabbB.max.y && aabbA.max.y > aabbB.min.y);
    }

    inline bool checkCircleCollision(const glm::vec2 &centerA, float radiusA, const glm::vec2 &centerB, float radiusB)
    {
        // Check if the distance between the centers is less than the sum of the radii
        return glm::distance(centerA, centerB) < (radiusA + radiusB);
    }

    inline bool checkAABBCircleCollision(const AABB &aabb, const glm::vec2 &circleCenter, float circleRadius)
    {
        // Find the closest point on the AABB to the circle center
        glm::vec2 closestPoint;
        closestPoint.x = glm::clamp(circleCenter.x, aabb.min.x, aabb.max.x);
        closestPoint.y = glm::clamp(circleCenter.y, aabb.min.y, aabb.max.y);

        // Calculate the distance between the circle center and the closest point
        float distance = glm::distance(circleCenter, closestPoint);

        // If the distance is less than or equal to the circle radius, they are colliding
        return distance <= circleRadius;
    }
}

#endif
