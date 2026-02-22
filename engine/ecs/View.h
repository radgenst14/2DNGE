#ifndef VIEW_H
#define VIEW_H

#pragma once

#include <vector>
#include "ComponentTypeID.h"

template <typename... Components>
class View
{
public:
    View(const EntityManager &em, const std::vector<EntityID> &smallest)
        : mEM(em), mSmallest(smallest) {}

    struct Iterator
    {
        const EntityManager &em;
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
            while (it != end && !em.hasAllComponents<Components...>(*it))
                ++it;
        }
    };

    Iterator begin() const
    {
        Iterator it{mEM, mSmallest.begin(), mSmallest.end()};
        it.skipNonMatching();
        return it;
    }

    Iterator end() const
    {
        return {mEM, mSmallest.end(), mSmallest.end()};
    }

private:
    const EntityManager &mEM;
    const std::vector<EntityID> &mSmallest;
};

#endif
