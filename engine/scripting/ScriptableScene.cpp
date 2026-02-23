#include "ScriptableScene.h"
#include "../core/Window.h"
#include "../core/ecs/EntityManager.h"
#include "../physics/PhysicsManager.h"
#include "../renderer/RenderManager.h"

ScriptableScene::ScriptableScene(const std::string &scriptPath, Window *window)
    : mScriptPath(scriptPath), mWindow(window)
{
}

ScriptableScene::~ScriptableScene()
{
    cleanup();
}

void ScriptableScene::init()
{
    // Clean up any previous init to prevent leaks on double-init
    if (mEntityManager)
    {
        cleanup();
    }

    mEntityManager = std::make_unique<EntityManager>();
    mPhysicsManager = std::make_unique<PhysicsManager>(mEntityManager.get());

    if (mWindow)
    {
        mRenderManager = std::make_unique<RenderManager>(mWindow, mEntityManager.get());
        EngineBindings::setRenderManager(mRenderManager.get());
        EngineBindings::setAssetManager(mRenderManager->getAssetManager());
    }

    EngineBindings::setEntityManager(mEntityManager.get());
    EngineBindings::setPhysicsManager(mPhysicsManager.get());

    if (!mScriptEngine.loadScript(mScriptPath))
    {
        SDL_Log("ScriptableScene failed to load: %s", mScriptPath.c_str());
        return;
    }

    mScriptEngine.callFunction("init");
}

void ScriptableScene::input(SDL_Event &event)
{
    mScriptEngine.callFunction("input");
}

void ScriptableScene::update(float dt)
{
    mScriptEngine.callFunction("update", dt);
}

void ScriptableScene::render()
{
    mScriptEngine.callFunction("render");
}

void ScriptableScene::cleanup()
{
    mScriptEngine.callFunction("cleanup");
    EngineBindings::setEntityManager(nullptr);
    EngineBindings::setPhysicsManager(nullptr);
    EngineBindings::setRenderManager(nullptr);
    EngineBindings::setAssetManager(nullptr);

    // Destroy managers that reference EntityManager first
    mRenderManager.reset();
    mPhysicsManager.reset();
    mEntityManager.reset();
}