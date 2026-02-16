#ifndef PHYSICSTESTSCENE_H
#define PHYSICSTESTSCENE_H

#pragma once

#include <memory>
#include <engine/core/IScene.h>
#include <engine/ecs/EntityManager.h>
#include <engine/physics/PhysicsManager.h>

class PhysicsTestScene : public IScene
{
public:
    void init() override;
    void input(SDL_Event &event) override;
    void update(float dt) override;
    void render(Renderer &renderer) override;
    void cleanup() override;

private:
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<PhysicsManager> mPhysicsManager;
};

#endif
