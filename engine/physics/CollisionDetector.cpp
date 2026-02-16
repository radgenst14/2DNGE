#include "CollisionDetector.h"
#include "../ecs/EntityManager.h"
#include "../ecs/components/Collider.h"
#include "../ecs/components/Transform.h"

CollisionDetector::~CollisionDetector()
{
}

CollisionDetector::AABB CollisionDetector::getColliderAABB(EntityID entity, ECS::Collider collider)
{
    AABB aabb;

    // Get the transform component to calculate the AABB based on the entity's position, scale, and collider size
    const auto &transform = mEntityManager->getComponent<ECS::Transform>(entity);
    glm::vec2 center = transform.position + collider.offset;

    glm::vec2 halfSize = (collider.size * transform.scale) * 0.5f;
    aabb.min = center - halfSize;
    aabb.max = center + halfSize;

    return aabb;
}

bool CollisionDetector::checkCollision(EntityID entityA, EntityID entityB)
{
    // If either entity doesn't have a collider, they can't collide
    if (!mEntityManager->hasComponent<ECS::Collider>(entityA) || !mEntityManager->hasComponent<ECS::Collider>(entityB))
    {
        return false;
    }

    // Get the colliders for both entities
    const auto &colliderA = mEntityManager->getComponent<ECS::Collider>(entityA);
    const auto &colliderB = mEntityManager->getComponent<ECS::Collider>(entityB);

    // Check what type of colliders we have and call the appropriate collision check
    if (colliderA.type == ECS::ColliderType::Box && colliderB.type == ECS::ColliderType::Box)
    {
        // For box-box collision, we can use AABB collision detection
        AABB aabbA = getColliderAABB(entityA, colliderA);
        AABB aabbB = getColliderAABB(entityB, colliderB);
        return checkAABBCollision(aabbA, aabbB);
    }
    else if (colliderA.type == ECS::ColliderType::Circle && colliderB.type == ECS::ColliderType::Circle)
    {
        // For circle-circle collision, we can use distance-based collision detection
        glm::vec2 centerA = mEntityManager->getComponent<ECS::Transform>(entityA).position + colliderA.offset;
        glm::vec2 centerB = mEntityManager->getComponent<ECS::Transform>(entityB).position + colliderB.offset;
        return checkCircleCollision(centerA, colliderA.radius, centerB, colliderB.radius);
    }
    else
    {
        // For box-circle collision, we can use a combination of AABB and circle collision detection
        AABB aabb = (colliderA.type == ECS::ColliderType::Box) ? getColliderAABB(entityA, colliderA) : getColliderAABB(entityB, colliderB);
        glm::vec2 circleCenter = (colliderA.type == ECS::ColliderType::Circle) ? (mEntityManager->getComponent<ECS::Transform>(entityA).position + colliderA.offset) : (mEntityManager->getComponent<ECS::Transform>(entityB).position + colliderB.offset);
        float circleRadius = (colliderA.type == ECS::ColliderType::Circle) ? colliderA.radius : colliderB.radius;
        return checkAABBCircleCollision(aabb, circleCenter, circleRadius);
    }

    return false;
}

bool CollisionDetector::checkAABBCollision(const AABB &aabbA, const AABB &aabbB)
{
    // Check if the AABBs overlap on both axes
    return (aabbA.min.x < aabbB.max.x && aabbA.max.x > aabbB.min.x &&
            aabbA.min.y < aabbB.max.y && aabbA.max.y > aabbB.min.y);
}

bool CollisionDetector::checkCircleCollision(const glm::vec2 &centerA, float radiusA, const glm::vec2 &centerB, float radiusB)
{
    // Check if the distance between the centers is less than the sum of the radii
    return glm::distance(centerA, centerB) < (radiusA + radiusB);
}

bool CollisionDetector::checkAABBCircleCollision(const AABB &aabb, const glm::vec2 &circleCenter, float circleRadius)
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