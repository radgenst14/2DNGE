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

Math::CollisionResult CollisionDetector::checkCollision(EntityID entityA, EntityID entityB)
{
    Math::CollisionResult result;

    if (!mEntityManager->hasComponent<ECS::Collider>(entityA) || !mEntityManager->hasComponent<ECS::Collider>(entityB))
    {
        return result;
    }

    const auto &colliderA = mEntityManager->getComponent<ECS::Collider>(entityA);
    const auto &colliderB = mEntityManager->getComponent<ECS::Collider>(entityB);

    if (colliderA.type == ECS::ColliderType::Box && colliderB.type == ECS::ColliderType::Box)
    {
        Math::AABB aabbA = getColliderAABB(entityA, colliderA);
        Math::AABB aabbB = getColliderAABB(entityB, colliderB);
        return Math::resolveAABBCollision(aabbA, aabbB);
    }
    else if (colliderA.type == ECS::ColliderType::Circle && colliderB.type == ECS::ColliderType::Circle)
    {
        glm::vec2 centerA = mEntityManager->getComponent<ECS::Transform>(entityA).position + colliderA.offset;
        glm::vec2 centerB = mEntityManager->getComponent<ECS::Transform>(entityB).position + colliderB.offset;
        return Math::resolveCircleCollision(centerA, colliderA.radius, centerB, colliderB.radius);
    }
    else
    {
        // Determine which is box and which is circle
        bool aIsBox = (colliderA.type == ECS::ColliderType::Box);

        EntityID boxEntity = aIsBox ? entityA : entityB;
        EntityID circleEntity = aIsBox ? entityB : entityA;
        const auto &boxCollider = aIsBox ? colliderA : colliderB;
        const auto &circleCollider = aIsBox ? colliderB : colliderA;

        Math::AABB aabb = getColliderAABB(boxEntity, boxCollider);
        glm::vec2 circleCenter = mEntityManager->getComponent<ECS::Transform>(circleEntity).position + circleCollider.offset;

        result = Math::resolveAABBCircleCollision(aabb, circleCenter, circleCollider.radius);

        // resolveAABBCircleCollision returns normal pointing from AABB toward circle.
        // If A is the box, we need to flip so normal pushes A away from B.
        if (aIsBox && result.isColliding)
        {
            result.normal = -result.normal;
        }

        return result;
    }
}