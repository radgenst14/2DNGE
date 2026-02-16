#ifndef COMPONENTPOOL_H
#define COMPONENTPOOL_H

#pragma once

#include <vector>
#include <cassert>
#include <limits>
#include "ComponentTypeID.h"

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
    virtual bool has(EntityID entity) const = 0;
    virtual bool remove(EntityID entity) = 0;
};

template <typename T>
class ComponentPool : public IComponentPool
{
public:
    static constexpr uint32_t INVALID = std::numeric_limits<uint32_t>::max();

    T &add(EntityID entity, const T &component)
    {
        assert(!has(entity) && "Entity already has this component");
        ensureSparseSize(entity);

        mSparse[entity] = static_cast<uint32_t>(mDense.size());
        mDense.push_back(component);
        mDenseToEntity.push_back(entity);

        return mDense.back();
    }

    T &get(EntityID entity)
    {
        assert(has(entity) && "Entity does not have this component");
        return mDense[mSparse[entity]];
    }

    const T &get(EntityID entity) const
    {
        assert(has(entity) && "Entity does not have this component");
        return mDense[mSparse[entity]];
    }

    bool has(EntityID entity) const override
    {
        return entity < mSparse.size() && mSparse[entity] != INVALID;
    }

    bool remove(EntityID entity)
    {
        if (!has(entity))
        {
            return false;
        }

        uint32_t index = mSparse[entity];
        uint32_t lastIndex = static_cast<uint32_t>(mDense.size() - 1);
        EntityID lastEntity = mDenseToEntity[lastIndex];

        // Move the last component to the removed spot
        mDense[index] = std::move(mDense[lastIndex]);
        mDenseToEntity[index] = lastEntity;
        mSparse[lastEntity] = index;

        // Remove the last component
        mDense.pop_back();
        mDenseToEntity.pop_back();
        mSparse[entity] = INVALID;

        return true;
    }

    /**
     * @brief Accessors for internal data structures (for iteration purposes)
     */
    const std::vector<EntityID> &getDenseToEntity() const { return mDenseToEntity; }
    const std::vector<T> &getDense() const { return mDense; }
    const std::vector<uint32_t> &getSparse() const { return mSparse; }

    std::vector<EntityID> &getDenseToEntity() { return mDenseToEntity; }
    std::vector<T> &getDense() { return mDense; }
    std::vector<uint32_t> &getSparse() { return mSparse; }

private:
    void ensureSparseSize(EntityID entity)
    {
        if (entity >= mSparse.size())
        {
            mSparse.resize(entity + 1, INVALID);
        }
    }

    std::vector<uint32_t> mSparse;        ///< EntityID -> index into mDense (or INVALID)
    std::vector<T> mDense;                ///< Contiguous component data
    std::vector<EntityID> mDenseToEntity; ///< Dense index -> EntityID
};

#endif
