#ifndef VIEW_H
#define VIEW_H

#pragma once

#include <vector>
#include <array>
#include "ComponentPool.h"

template <typename... Components>
class View
{
public:
    View(const std::vector<EntityID> &smallest,
         std::array<const IComponentPool *, sizeof...(Components)> pools)
        : mSmallest(smallest), mPools(pools) {}

    struct Iterator
    {
        std::array<const IComponentPool *, sizeof...(Components)> pools;
        typename std::vector<EntityID>::const_iterator it;
        typename std::vector<EntityID>::const_iterator end;

        Iterator &operator++()
        {
            ++it;
            skipNonMatching();
            return *this;
        }

        EntityID operator*() const { return *it; }
        bool operator!=(const Iterator &other) const { return it != other.it; }

        void skipNonMatching()
        {
            while (it != end && !hasAll(*it))
                ++it;
        }

    private:
        bool hasAll(EntityID entity) const
        {
            for (const IComponentPool *pool : pools)
            {
                if (!pool || !pool->has(entity))
                    return false;
            }
            return true;
        }
    };

    Iterator begin() const
    {
        Iterator it{mPools, mSmallest.begin(), mSmallest.end()};
        it.skipNonMatching();
        return it;
    }

    Iterator end() const
    {
        return {mPools, mSmallest.end(), mSmallest.end()};
    }

private:
    const std::vector<EntityID> &mSmallest;
    std::array<const IComponentPool *, sizeof...(Components)> mPools;
};

#endif
