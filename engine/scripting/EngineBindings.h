#ifndef ENGINEBINDINGS_H
#define ENGINEBINDINGS_H

#pragma once

class EntityManager;
class PhysicsManager;
class RenderManager;

namespace EngineBindings
{
    void setEntityManager(EntityManager *em);
    void setPhysicsManager(PhysicsManager *pm);
    void setRenderManager(RenderManager *rm);

    EntityManager *getEntityManager();
    PhysicsManager *getPhysicsManager();
    RenderManager *getRenderManager();
}

#endif