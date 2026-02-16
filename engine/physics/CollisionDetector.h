#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#pragma once

#include <glm/glm.hpp>
#include "../ecs/ComponentTypeID.h"
#include "../ecs/components/Collider.h"

class EntityManager;

class CollisionDetector
{
private:
    struct AABB
    {
        glm::vec2 min;
        glm::vec2 max;
    };

public:
    CollisionDetector(EntityManager *entityManager) : mEntityManager(entityManager) {};
    ~CollisionDetector();

    /* Delete copy constructor and assignment operator */
    CollisionDetector(const CollisionDetector &) = delete;
    CollisionDetector &operator=(const CollisionDetector &) = delete;

    AABB getColliderAABB(EntityID entity, ECS::Collider collider);

    bool checkCollision(EntityID entityA, EntityID entityB);
    bool checkAABBCollision(const AABB &aabbA, const AABB &aabbB);
    bool checkCircleCollision(const glm::vec2 &centerA, float radiusA, const glm::vec2 &centerB, float radiusB);
    bool checkAABBCircleCollision(const AABB &aabb, const glm::vec2 &circleCenter, float circleRadius);

private:
    EntityManager *mEntityManager;
};

#endif