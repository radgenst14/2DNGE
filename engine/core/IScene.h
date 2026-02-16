#ifndef SCENE_H
#define SCENE_H

#pragma once
#include <SDL.h>

class Renderer;

class IScene
{
public:
    virtual ~IScene() = default;
    virtual void Init() = 0; // Create entities, load resources
    virtual void Input(SDL_Event &event) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(Renderer &renderer) = 0;
    virtual void Cleanup() = 0; // Destroy entities
};

#endif