#include "RenderBindings.h"
#include "../EngineBindings.h"
#include "../../renderer/RenderManager.h"
#include "../../renderer/Camera.h"

void registerRenderBindings(py::module_ &m)
{
    m.def("render", []()
          {
              auto *rm = EngineBindings::getRenderManager();
              if (rm) rm->render();
          }, "Render the current frame, including all sprites and other renderable entities.");

    m.def("set_camera_position", [](float x, float y)
          {
              auto *rm = EngineBindings::getRenderManager();
              if (rm) rm->getCamera().setPosition({x, y});
          }, py::arg("x"), py::arg("y"), "Set the camera position in world coordinates.");

    m.def("get_camera_position", []() -> py::tuple
          {
              auto *rm = EngineBindings::getRenderManager();
              if (!rm) return py::make_tuple(0.0f, 0.0f);
              auto pos = rm->getCamera().getPosition();
              return py::make_tuple(pos.x, pos.y);
          }, "Get the current camera position as (x, y).");

    m.def("move_camera", [](float dx, float dy)
          {
              auto *rm = EngineBindings::getRenderManager();
              if (rm) rm->getCamera().move({dx, dy});
          }, py::arg("dx"), py::arg("dy"), "Move the camera by a delta in world coordinates.");

    m.def("set_camera_zoom", [](float zoom)
          {
              auto *rm = EngineBindings::getRenderManager();
              if (rm) rm->getCamera().setZoom(zoom);
          }, py::arg("zoom"), "Set the camera zoom level.");

    m.def("get_camera_zoom", []() -> float
          {
              auto *rm = EngineBindings::getRenderManager();
              if (!rm) return 1.0f;
              return rm->getCamera().getZoom();
          }, "Get the current camera zoom level.");
}
