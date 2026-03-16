#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#pragma once

#include "../core/ecs/ComponentTypeID.h"
#include "Math.h"

class EntityManager;

class CollisionHandler
{
public:
    CollisionHandler(EntityManager *entityManager) : mEntityManager(entityManager) {};
    ~CollisionHandler();

    /* Delete copy constructor and assignment operator */
    CollisionHandler(const CollisionHandler &) = delete;
    CollisionHandler &operator=(const CollisionHandler &) = delete;

    void resolveCollision(EntityID entityA, EntityID entityB, const Math::CollisionResult &result);

private:
    EntityManager *mEntityManager;
};

#endif