#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#pragma once

#include <cstdint>

using EntityID = uint32_t;

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    /* Delete default constructor and assignment operator */
    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;

private:
    EntityID mNextEntityID = 0; ///< The next available entity ID
};

#endif