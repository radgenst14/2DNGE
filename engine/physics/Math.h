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

    struct CollisionResult
    {
        bool isColliding{false};

        glm::vec2 normal{0.0f, 0.0f}; // Direction to push A away from B
        float penetration{0.0f};      // How deep the overlap is
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

    inline CollisionResult resolveAABBCollision(const AABB &aabbA, const AABB &aabbB)
    {
        CollisionResult result;

        // Compute overlap on each axis
        float overlapX1 = aabbA.max.x - aabbB.min.x; // A pushing right into B's left
        float overlapX2 = aabbB.max.x - aabbA.min.x; // B pushing right into A's left
        float overlapY1 = aabbA.max.y - aabbB.min.y; // A pushing down into B's top
        float overlapY2 = aabbB.max.y - aabbA.min.y; // B pushing down into A's top

        // If any overlap is negative or zero, no collision
        if (overlapX1 <= 0 || overlapX2 <= 0 || overlapY1 <= 0 || overlapY2 <= 0)
            return result;

        // Find minimum overlap on each axis and its direction
        // signX/signY indicate the direction to push A away from B
        float overlapX = (overlapX1 < overlapX2) ? overlapX1 : overlapX2;
        float signX = (overlapX1 < overlapX2) ? -1.0f : 1.0f;

        float overlapY = (overlapY1 < overlapY2) ? overlapY1 : overlapY2;
        float signY = (overlapY1 < overlapY2) ? -1.0f : 1.0f;

        // Pick the axis with least penetration
        result.isColliding = true;
        if (overlapX < overlapY)
        {
            result.normal = glm::vec2(signX, 0.0f);
            result.penetration = overlapX;
        }
        else
        {
            result.normal = glm::vec2(0.0f, signY);
            result.penetration = overlapY;
        }

        return result;
    }

    inline CollisionResult resolveCircleCollision(const glm::vec2 &centerA, float radiusA,
                                                  const glm::vec2 &centerB, float radiusB)
    {
        CollisionResult result;

        glm::vec2 delta = centerA - centerB;
        float distSquared = glm::dot(delta, delta);
        float radiusSum = radiusA + radiusB;

        if (distSquared >= radiusSum * radiusSum)
            return result;

        float distance = std::sqrt(distSquared);

        result.isColliding = true;
        if (distance > 0.0f)
        {
            result.normal = delta / distance; // Normalized direction from B to A
            result.penetration = radiusSum - distance;
        }
        else
        {
            // Concentric circles — pick an arbitrary separation axis
            result.normal = glm::vec2(1.0f, 0.0f);
            result.penetration = radiusSum;
        }

        return result;
    }

    inline CollisionResult resolveAABBCircleCollision(const AABB &aabb, const glm::vec2 &circleCenter,
                                                      float circleRadius)
    {
        CollisionResult result;

        glm::vec2 closestPoint;
        closestPoint.x = glm::clamp(circleCenter.x, aabb.min.x, aabb.max.x);
        closestPoint.y = glm::clamp(circleCenter.y, aabb.min.y, aabb.max.y);

        glm::vec2 delta = circleCenter - closestPoint;
        float distSquared = glm::dot(delta, delta);

        if (distSquared > circleRadius * circleRadius)
            return result;

        result.isColliding = true;
        float distance = std::sqrt(distSquared);

        if (distance > 0.0f)
        {
            // Circle center is outside the AABB
            result.normal = delta / distance; // Points from AABB surface toward circle center
            result.penetration = circleRadius - distance;
        }
        else
        {
            // Circle center is inside the AABB — find nearest edge
            float distToRight = aabb.max.x - circleCenter.x;
            float distToLeft = circleCenter.x - aabb.min.x;
            float distToTop = circleCenter.y - aabb.min.y;
            float distToBottom = aabb.max.y - circleCenter.y;

            float minDist = distToRight;
            result.normal = glm::vec2(1.0f, 0.0f);

            if (distToLeft < minDist)
            {
                minDist = distToLeft;
                result.normal = glm::vec2(-1.0f, 0.0f);
            }
            if (distToTop < minDist)
            {
                minDist = distToTop;
                result.normal = glm::vec2(0.0f, -1.0f);
            }
            if (distToBottom < minDist)
            {
                minDist = distToBottom;
                result.normal = glm::vec2(0.0f, 1.0f);
            }

            result.penetration = circleRadius + minDist;
        }

        return result;
    }
}

#endif
