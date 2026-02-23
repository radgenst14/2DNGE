#include <pybind11/embed.h>
#include "EngineBindings.h"
#include "bindings/ECSBindings.h"
#include "bindings/PhysicsBindings.h"
#include "bindings/RenderBindings.h"

namespace py = pybind11;

static EntityManager *sEntityManager = nullptr;
static PhysicsManager *sPhysicsManager = nullptr;
static RenderManager *sRenderManager = nullptr;
static InputManager *sInputManager = nullptr;

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

void EngineBindings::setInputManager(InputManager *im)
{
    sInputManager = im;
}

InputManager *EngineBindings::getInputManager()
{
    return sInputManager;
}

PYBIND11_EMBEDDED_MODULE(engine, m)
{
    registerECSBindings(m);
    registerPhysicsBindings(m);
    registerRenderBindings(m);
}