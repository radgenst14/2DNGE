#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#pragma once

#include <memory>

class Renderer;

class RenderManager
{
public:
    RenderManager();
    ~RenderManager();

    /* Delete  copy constructor and assignment operator */
    RenderManager(const RenderManager &) = delete;
    RenderManager &operator=(const RenderManager &) = delete;

private:
    std::shared_ptr<Renderer> mRenderer;
};

#endif