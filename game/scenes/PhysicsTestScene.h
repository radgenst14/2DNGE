#ifndef PHYSICSTESTSCENE_H
#define PHYSICSTESTSCENE_H

#pragma once

#include <engine/core/IScene.h>

class PhysicsTestScene : public IScene
{
public:
    void Init() override;
    void Input(SDL_Event &event) override;
    void Update(float dt) override;
    void Render(Renderer &renderer) override;
    void Cleanup() override;
};

#endif
