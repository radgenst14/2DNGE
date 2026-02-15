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
};

template <typename T>
class ComponentPool : public IComponentPool
{
public:
    static constexpr uint32_t INVALID = std::numeric_limits<uint32_t>::max();

    T& add(EntityID entity, const T& component)
    {
        assert(!has(entity) && "Entity already has this component");
        ensureSparseSize(entity);

        mSparse[entity] = static_cast<uint32_t>(mDense.size());
        mDense.push_back(component);
        mDenseToEntity.push_back(entity);

        return mDense.back();
    }

    T& get(EntityID entity)
    {
        assert(has(entity) && "Entity does not have this component");
        return mDense[mSparse[entity]];
    }

    const T& get(EntityID entity) const
    {
        assert(has(entity) && "Entity does not have this component");
        return mDense[mSparse[entity]];
    }

    bool has(EntityID entity) const override
    {
        return entity < mSparse.size() && mSparse[entity] != INVALID;
    }

private:
    void ensureSparseSize(EntityID entity)
    {
        if (entity >= mSparse.size())
        {
            mSparse.resize(entity + 1, INVALID);
        }
    }

    std::vector<uint32_t> mSparse;       ///< EntityID -> index into mDense (or INVALID)
    std::vector<T> mDense;                ///< Contiguous component data
    std::vector<EntityID> mDenseToEntity; ///< Dense index -> EntityID
};

#endif
