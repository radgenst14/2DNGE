#include "PhysicsBindings.h"
#include "../EngineBindings.h"
#include "../../physics/PhysicsManager.h"

void registerPhysicsBindings(py::module_ &m)
{
    m.def("physics_update", [](float dt)
          { EngineBindings::getPhysicsManager()->update(dt); }, "Update the physics simulation by a given time step (dt). This will move all physics-enabled entities according to their velocities and handle collisions.");
}