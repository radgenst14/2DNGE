#include "PhysicsBindings.h"
#include "../EngineBindings.h"
#include "../../physics/PhysicsManager.h"

void registerPhysicsBindings(py::module_ &m)
{
    m.def("physics_update", [](float dt)
          { EngineBindings::getPhysicsManager()->update(dt); });
}