#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#pragma once

class CollisionHandler
{
public:
    CollisionHandler();
    ~CollisionHandler();

    /* Delete copy constructor and assignment operator */
    CollisionHandler(const CollisionHandler &) = delete;
    CollisionHandler &operator=(const CollisionHandler &) = delete;

private:
};

#endif