#ifndef ENTITY_H
#define ENTITY_H

#pragma once

#include <cstdint>
#include <unordered_map>
#include <typeindex>
#include <any>

using EntityID = uint32_t;

class Entity
{
public:
    Entity(EntityID id) : mId(id) {};
    ~Entity();

    /* Delete default constructor and assignment operator */
    Entity(const Entity &) = delete;
    Entity &operator=(const Entity &) = delete;

    /**
     * @brief Adds a component of the specified type to the entity.
     * @tparam T The type of the component to add.
     * @param component The component instance to add to the entity.
     */
    template <typename T>
    void addComponent(const T &component)
    {
        mComponents[std::type_index(typeid(T))] = component;
    }

    /**
     * @brief Gets a component of the specified type from the entity.
     * @tparam T The type of the component to get.
     * @return A reference to the component instance.
     */
    template <typename T>
    const T &getComponent() const
    {
        return std::any_cast<const T &>(mComponents.at(std::type_index(typeid(T))));
    }

    /**
     *@brief Checks if the entity has a component of the specified type.
     *@tparam T The type of the component to check for.
     *@return true if the entity has the component, false otherwise.
     */
    template <typename T>
    bool hasComponent() const
    {
        return mComponents.find(std::type_index(typeid(T))) != mComponents.end();
    }

private:
    EntityID mId;                                              ///< Unique identifier for the entity
    std::unordered_map<std::type_index, std::any> mComponents; ///< Map of component type to component instance
};

#endif