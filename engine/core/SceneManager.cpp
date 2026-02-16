#include "SceneManager.h"

void SceneManager::SetScene(std::unique_ptr<IScene> scene)
{
    mActiveScene = std::move(scene);
}

IScene *SceneManager::GetActiveScene()
{
    return mActiveScene.get();
}