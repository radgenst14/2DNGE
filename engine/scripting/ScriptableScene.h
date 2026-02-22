#ifndef SCRIPTABLESCENE_H
#define SCRIPTABLESCENE_H

#pragma once

#include <string>
#include <memory>
#include "../core/IScene.h"
#include "ScriptEngine.h"
#include "EngineBindings.h"

class EntityManager;
class PhysicsManager;

class ScriptableScene : public IScene
{
public:
    explicit ScriptableScene(const std::string &scriptPath);
    ~ScriptableScene();

    /* Delete copy constructor and assignment operator */
    ScriptableScene(const ScriptableScene &) = delete;
    ScriptableScene &operator=(const ScriptableScene &) = delete;

    void init() override;
    void input(SDL_Event &event) override;
    void update(float dt) override;
    void render(Renderer &renderer) override;
    void cleanup() override;

private:
    std::string mScriptPath;
    ScriptEngine mScriptEngine;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<PhysicsManager> mPhysicsManager;
};

#endif