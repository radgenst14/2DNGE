#include "RenderBindings.h"
#include "../EngineBindings.h"
#include "../../renderer/RenderManager.h"

void RegisterRenderBindings(py::module_ &m)
{
    m.def("graphics_update", []()
          { return 0; });
}
