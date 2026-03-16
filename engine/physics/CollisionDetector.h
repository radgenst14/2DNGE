#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#pragma once

#include <glm/glm.hpp>
#include "../core/ecs/ComponentTypeID.h"
#include "../core/ecs/components/Collider.h"
#include "Math.h"

class EntityManager;

class CollisionDetector
{
public:
    CollisionDetector(EntityManager *entityManager) : mEntityManager(entityManager) {};
    ~CollisionDetector();

    /* Delete copy constructor and assignment operator */
    CollisionDetector(const CollisionDetector &) = delete;
    CollisionDetector &operator=(const CollisionDetector &) = delete;

    Math::AABB getColliderAABB(EntityID entity, ECS::Collider collider);

    Math::CollisionResult checkCollision(EntityID entityA, EntityID entityB);

private:
    EntityManager *mEntityManager;
};

#endif