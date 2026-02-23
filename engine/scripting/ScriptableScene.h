#ifndef SCRIPTABLESCENE_H
#define SCRIPTABLESCENE_H

#pragma once

#include <string>
#include <memory>
#include "../core/IScene.h"
#include "ScriptEngine.h"
#include "EngineBindings.h"

class Window;
class EntityManager;
class PhysicsManager;
class RenderManager;

class ScriptableScene : public IScene
{
public:
    ScriptableScene(const std::string &scriptPath, Window *window);
    ~ScriptableScene();

    /* Delete copy constructor and assignment operator */
    ScriptableScene(const ScriptableScene &) = delete;
    ScriptableScene &operator=(const ScriptableScene &) = delete;

    void init() override;
    void input(SDL_Event &event) override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;

private:
    std::string mScriptPath;
    Window *mWindow;
    ScriptEngine mScriptEngine;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<PhysicsManager> mPhysicsManager;
    std::unique_ptr<RenderManager> mRenderManager;
};

#endif