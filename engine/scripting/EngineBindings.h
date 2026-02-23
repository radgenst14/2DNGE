#ifndef ENGINEBINDINGS_H
#define ENGINEBINDINGS_H

#pragma once

class EntityManager;
class PhysicsManager;
class RenderManager;
class InputManager;
class AssetManager;

namespace EngineBindings
{
    void setEntityManager(EntityManager *em);
    void setPhysicsManager(PhysicsManager *pm);
    void setRenderManager(RenderManager *rm);
    void setInputManager(InputManager *im);
    void setAssetManager(AssetManager *am);

    EntityManager *getEntityManager();
    PhysicsManager *getPhysicsManager();
    RenderManager *getRenderManager();
    InputManager *getInputManager();
    AssetManager *getAssetManager();
}

#endif