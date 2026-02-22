#include "ScriptableScene.h"
#include "../ecs/EntityManager.h"
#include "../physics/PhysicsManager.h"

ScriptableScene::ScriptableScene(const std::string &scriptPath)
    : mScriptPath(scriptPath)
{
}

ScriptableScene::~ScriptableScene() = default;

void ScriptableScene::init()
{
    mEntityManager = std::make_unique<EntityManager>();
    mPhysicsManager = std::make_unique<PhysicsManager>(mEntityManager.get());

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
}

void ScriptableScene::update(float dt)
{
    mScriptEngine.callFunction("update", dt);
}

void ScriptableScene::render(Renderer &renderer)
{
}

void ScriptableScene::cleanup()
{
    mScriptEngine.callFunction("cleanup");
    EngineBindings::setEntityManager(nullptr);
    EngineBindings::setPhysicsManager(nullptr);
}