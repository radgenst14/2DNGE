#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include "ComponentTypeID.h"
#include "ComponentPool.h"

class EntityManager
{
public:
    EntityManager() = default;
    ~EntityManager() = default;

    /* Delete copy constructor and assignment operator */
    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;

    EntityID createEntity();
    bool deleteEntity(EntityID entity);

    template <typename T>
    T &addComponent(EntityID entity, const T &component)
    {
        auto &pool = getOrCreatePool<T>();
        return pool.add(entity, component);
    }

    template <typename T>
    T &getComponent(EntityID entity)
    {
        return getOrCreatePool<T>().get(entity);
    }

    template <typename T>
    const T &getComponent(EntityID entity) const
    {
        return getPool<T>().get(entity);
    }

    template <typename T>
    bool hasComponent(EntityID entity) const
    {
        ComponentTypeID id = getComponentTypeID<T>();
        if (id >= mPools.size() || !mPools[id])
            return false;
        return static_cast<const ComponentPool<T> *>(mPools[id].get())->has(entity);
    }

    template <typename T>
    ComponentPool<T> &getComponentPool()
    {
        return getOrCreatePool<T>();
    }

    template <typename T>
    const ComponentPool<T> &getComponentPool() const
    {
        return getPool<T>();
    }

private:
    template <typename T>
    ComponentPool<T> &getOrCreatePool()
    {
        ComponentTypeID id = getComponentTypeID<T>();
        if (id >= mPools.size())
        {
            mPools.resize(id + 1);
        }
        if (!mPools[id])
        {
            mPools[id] = std::make_unique<ComponentPool<T>>();
        }
        return *static_cast<ComponentPool<T> *>(mPools[id].get());
    }

    template <typename T>
    const ComponentPool<T> &getPool() const
    {
        ComponentTypeID id = getComponentTypeID<T>();
        assert(id < mPools.size() && mPools[id] && "Pool does not exist for this type");
        return *static_cast<const ComponentPool<T> *>(mPools[id].get());
    }

    EntityID mNextEntityID = 0;
    std::vector<std::unique_ptr<IComponentPool>> mPools;
};

#endif
