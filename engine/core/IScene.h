#ifndef SCENE_H
#define SCENE_H

#pragma once
#include <SDL.h>

class IScene
{
public:
    virtual ~IScene() = default;
    virtual void init() = 0; // Create entities, load resources
    virtual void input(SDL_Event &event) = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0; // Destroy entities
};

#endif