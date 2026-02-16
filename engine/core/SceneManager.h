#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#pragma once

#include <memory>
#include "IScene.h"

class SceneManager
{
public:
    void setScene(std::unique_ptr<IScene> scene);
    IScene *getActiveScene();

private:
    std::unique_ptr<IScene> mActiveScene;
};

#endif