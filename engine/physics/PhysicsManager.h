#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#pragma once

class EntityManager;

class PhysicsManager
{
public:
    PhysicsManager(EntityManager *entityManager) : mEntityManager(entityManager) {};
    ~PhysicsManager();

    /* Delete copy constructor and assignment operator */
    PhysicsManager(const PhysicsManager &) = delete;
    PhysicsManager &operator=(const PhysicsManager &) = delete;

    void update(float dt);

private:
    EntityManager *mEntityManager; ///< Pointer to the EntityManager for accessing entities and their components
};

#endif