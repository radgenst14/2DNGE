#include <pybind11/embed.h>
#include "EngineBindings.h"
#include "bindings/ECSBindings.h"
#include "bindings/PhysicsBindings.h"

namespace py = pybind11;

static EntityManager *sEntityManager = nullptr;
static PhysicsManager *sPhysicsManager = nullptr;
static RenderManager *sRenderManager = nullptr;

void EngineBindings::setEntityManager(EntityManager *em)
{
    sEntityManager = em;
}

EntityManager *EngineBindings::getEntityManager()
{
    return sEntityManager;
}

void EngineBindings::setPhysicsManager(PhysicsManager *pm)
{
    sPhysicsManager = pm;
}

PhysicsManager *EngineBindings::getPhysicsManager()
{
    return sPhysicsManager;
}

void EngineBindings::setRenderManager(RenderManager *rm)
{
    sRenderManager = rm;
}

RenderManager *EngineBindings::getRenderManager()
{
    return sRenderManager;
}

PYBIND11_EMBEDDED_MODULE(engine, m)
{
    registerECSBindings(m);
    registerPhysicsBindings(m);
}