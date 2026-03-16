#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#pragma once

#include <memory>

class EntityManager;
class CollisionDetector;
class CollisionHandler;

class PhysicsManager
{
public:
    PhysicsManager(EntityManager *entityManager);
    ~PhysicsManager();

    /* Delete copy constructor and assignment operator */
    PhysicsManager(const PhysicsManager &) = delete;
    PhysicsManager &operator=(const PhysicsManager &) = delete;

    void update(float dt);

private:
    EntityManager *mEntityManager;                         ///< Pointer to the EntityManager for accessing entities and their components
    std::unique_ptr<CollisionDetector> mCollisionDetector; ///< Pointer to the CollisionDetector for checking collisions
    std::unique_ptr<CollisionHandler> mCollisionHandler;   ///< Pointer to the CollisionHandler for resolving collisions
};

#endif