#include "CollisionDetector.h"
#include "../core/ecs/EntityManager.h"
#include "../core/ecs/components/Collider.h"
#include "../core/ecs/components/Transform.h"

CollisionDetector::~CollisionDetector()
{
}

Math::AABB CollisionDetector::getColliderAABB(EntityID entity, ECS::Collider collider)
{
    Math::AABB aabb;

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
        Math::AABB aabbA = getColliderAABB(entityA, colliderA);
        Math::AABB aabbB = getColliderAABB(entityB, colliderB);
        return Math::checkAABBCollision(aabbA, aabbB);
    }
    else if (colliderA.type == ECS::ColliderType::Circle && colliderB.type == ECS::ColliderType::Circle)
    {
        // For circle-circle collision, we can use distance-based collision detection
        glm::vec2 centerA = mEntityManager->getComponent<ECS::Transform>(entityA).position + colliderA.offset;
        glm::vec2 centerB = mEntityManager->getComponent<ECS::Transform>(entityB).position + colliderB.offset;
        return Math::checkCircleCollision(centerA, colliderA.radius, centerB, colliderB.radius);
    }
    else
    {
        // For box-circle collision, we can use a combination of AABB and circle collision detection
        Math::AABB aabb = (colliderA.type == ECS::ColliderType::Box) ? getColliderAABB(entityA, colliderA) : getColliderAABB(entityB, colliderB);
        glm::vec2 circleCenter = (colliderA.type == ECS::ColliderType::Circle) ? (mEntityManager->getComponent<ECS::Transform>(entityA).position + colliderA.offset) : (mEntityManager->getComponent<ECS::Transform>(entityB).position + colliderB.offset);
        float circleRadius = (colliderA.type == ECS::ColliderType::Circle) ? colliderA.radius : colliderB.radius;
        return Math::checkAABBCircleCollision(aabb, circleCenter, circleRadius);
    }

    return false;
}