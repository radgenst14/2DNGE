#ifndef COMPONENTTYPEID_H
#define COMPONENTTYPEID_H

#pragma once

#include <cstdint>

using EntityID = uint32_t;
using ComponentTypeID = uint32_t;

inline ComponentTypeID getNextComponentTypeID()
{
    static ComponentTypeID lastID = 0;
    return lastID++;
}

template <typename T>
ComponentTypeID getComponentTypeID()
{
    static const ComponentTypeID typeID = getNextComponentTypeID();
    return typeID;
}

#endif
