#ifndef ENGINEBINDINGS_H
#define ENGINEBINDINGS_H

#pragma once

class EntityManager;
class PhysicsManager;
class RenderManager;
class InputManager;

namespace EngineBindings
{
    void setEntityManager(EntityManager *em);
    void setPhysicsManager(PhysicsManager *pm);
    void setRenderManager(RenderManager *rm);
    void setInputManager(InputManager *im);

    EntityManager *getEntityManager();
    PhysicsManager *getPhysicsManager();
    RenderManager *getRenderManager();
    InputManager *getInputManager();
}

#endif