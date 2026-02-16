#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#pragma once

#include <memory>
#include "IScene.h"

class SceneManager
{
public:
    void SetScene(std::unique_ptr<IScene> scene);
    IScene *GetActiveScene();

private:
    std::unique_ptr<IScene> mActiveScene;
};

#endif