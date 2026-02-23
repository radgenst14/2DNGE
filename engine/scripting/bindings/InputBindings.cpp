#include "InputBindings.h"
#include "../EngineBindings.h"
#include "../../core/InputManager.h"

void registerInputBindings(py::module_ &m)
{
      m.def("is_key_down", [](int scancode)
            { return EngineBindings::getInputManager()->isKeyDown(static_cast<SDL_Scancode>(scancode)); }, "Check if a key is currently being held down. Returns True if the key corresponding to the given scancode is pressed, False otherwise.");

      m.def("is_key_pressed", [](int scancode)
            { return EngineBindings::getInputManager()->isKeyPressed(static_cast<SDL_Scancode>(scancode)); }, "Check if a key was pressed this frame. Returns True if the key corresponding to the given scancode was pressed this frame, False otherwise.");

      m.def("is_key_released", [](int scancode)
            { return EngineBindings::getInputManager()->isKeyReleased(static_cast<SDL_Scancode>(scancode)); }, "Check if a key was released this frame. Returns True if the key corresponding to the given scancode was released this frame, False otherwise.");

      // Expose common scancodes as module-level constants
      m.attr("KEY_A") = py::int_(SDL_SCANCODE_A);
      m.attr("KEY_B") = py::int_(SDL_SCANCODE_B);
      m.attr("KEY_C") = py::int_(SDL_SCANCODE_C);
      m.attr("KEY_D") = py::int_(SDL_SCANCODE_D);
      m.attr("KEY_E") = py::int_(SDL_SCANCODE_E);
      m.attr("KEY_F") = py::int_(SDL_SCANCODE_F);
      m.attr("KEY_G") = py::int_(SDL_SCANCODE_G);
      m.attr("KEY_H") = py::int_(SDL_SCANCODE_H);
      m.attr("KEY_I") = py::int_(SDL_SCANCODE_I);
      m.attr("KEY_J") = py::int_(SDL_SCANCODE_J);
      m.attr("KEY_K") = py::int_(SDL_SCANCODE_K);
      m.attr("KEY_L") = py::int_(SDL_SCANCODE_L);
      m.attr("KEY_M") = py::int_(SDL_SCANCODE_M);
      m.attr("KEY_N") = py::int_(SDL_SCANCODE_N);
      m.attr("KEY_O") = py::int_(SDL_SCANCODE_O);
      m.attr("KEY_P") = py::int_(SDL_SCANCODE_P);
      m.attr("KEY_Q") = py::int_(SDL_SCANCODE_Q);
      m.attr("KEY_R") = py::int_(SDL_SCANCODE_R);
      m.attr("KEY_S") = py::int_(SDL_SCANCODE_S);
      m.attr("KEY_T") = py::int_(SDL_SCANCODE_T);
      m.attr("KEY_U") = py::int_(SDL_SCANCODE_U);
      m.attr("KEY_V") = py::int_(SDL_SCANCODE_V);
      m.attr("KEY_W") = py::int_(SDL_SCANCODE_W);
      m.attr("KEY_X") = py::int_(SDL_SCANCODE_X);
      m.attr("KEY_Y") = py::int_(SDL_SCANCODE_Y);
      m.attr("KEY_Z") = py::int_(SDL_SCANCODE_Z);

      m.attr("KEY_0") = py::int_(SDL_SCANCODE_0);
      m.attr("KEY_1") = py::int_(SDL_SCANCODE_1);
      m.attr("KEY_2") = py::int_(SDL_SCANCODE_2);
      m.attr("KEY_3") = py::int_(SDL_SCANCODE_3);
      m.attr("KEY_4") = py::int_(SDL_SCANCODE_4);
      m.attr("KEY_5") = py::int_(SDL_SCANCODE_5);
      m.attr("KEY_6") = py::int_(SDL_SCANCODE_6);
      m.attr("KEY_7") = py::int_(SDL_SCANCODE_7);
      m.attr("KEY_8") = py::int_(SDL_SCANCODE_8);
      m.attr("KEY_9") = py::int_(SDL_SCANCODE_9);

      m.attr("KEY_SPACE") = py::int_(SDL_SCANCODE_SPACE);
      m.attr("KEY_RETURN") = py::int_(SDL_SCANCODE_RETURN);
      m.attr("KEY_ESCAPE") = py::int_(SDL_SCANCODE_ESCAPE);
      m.attr("KEY_LSHIFT") = py::int_(SDL_SCANCODE_LSHIFT);
      m.attr("KEY_RSHIFT") = py::int_(SDL_SCANCODE_RSHIFT);
      m.attr("KEY_LCTRL") = py::int_(SDL_SCANCODE_LCTRL);
      m.attr("KEY_RCTRL") = py::int_(SDL_SCANCODE_RCTRL);
      m.attr("KEY_TAB") = py::int_(SDL_SCANCODE_TAB);

      m.attr("KEY_UP") = py::int_(SDL_SCANCODE_UP);
      m.attr("KEY_DOWN") = py::int_(SDL_SCANCODE_DOWN);
      m.attr("KEY_LEFT") = py::int_(SDL_SCANCODE_LEFT);
      m.attr("KEY_RIGHT") = py::int_(SDL_SCANCODE_RIGHT);
}