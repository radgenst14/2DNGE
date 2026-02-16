#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#pragma once

#include <glm/glm.hpp>

class EntityManager;

class CollisionDetector
{
public:
    CollisionDetector(EntityManager *entityManager) : mEntityManager(entityManager) {};
    ~CollisionDetector();

    /* Delete copy constructor and assignment operator */
    CollisionDetector(const CollisionDetector &) = delete;
    CollisionDetector &operator=(const CollisionDetector &) = delete;

private:
    EntityManager *mEntityManager;
    struct AABB
    {
        glm::vec2 min;
        glm::vec2 max;
    };
};

#endif