#include "EntityManager.h"

EntityID EntityManager::createEntity()
{
    return mNextEntityID++;
}

bool EntityManager::deleteEntity(EntityID entity)
{
    // Mark the entity as deleted by removing its components from all pools
    for (auto &pool : mPools)
    {
        if (pool)
        {
            pool->remove(entity); // This will remove the component if it exists
        }
    }
    return true;
}