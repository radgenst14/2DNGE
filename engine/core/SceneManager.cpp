#include "SceneManager.h"

void SceneManager::setScene(std::unique_ptr<IScene> scene)
{
    if (mActiveScene)
    {
        mActiveScene->cleanup();
    }

    mActiveScene = std::move(scene);
    mActiveScene->init();
}

IScene *SceneManager::getActiveScene()
{
    return mActiveScene.get();
}