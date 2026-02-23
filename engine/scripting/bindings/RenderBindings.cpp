#include "RenderBindings.h"
#include "../EngineBindings.h"
#include "../../renderer/RenderManager.h"

void registerRenderBindings(py::module_ &m)
{
    m.def("render", []()
          {
              auto *rm = EngineBindings::getRenderManager();
              if (rm) rm->render();
          }, "Render the current frame, including all sprites and other renderable entities.");
}
