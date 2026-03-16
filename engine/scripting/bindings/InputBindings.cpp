#include "InputBindings.h"
#include "../EngineBindings.h"
#include "../../core/InputManager.h"

#define BIND_KEY(name, scancode) m.attr(name) = py::int_(static_cast<int>(scancode))

void registerInputBindings(py::module_ &m)
{
      m.def("is_key_down", [](int scancode)
            { return EngineBindings::getInputManager()->isKeyDown(static_cast<SDL_Scancode>(scancode)); }, "Check if a key is currently being held down.");

      m.def("is_key_pressed", [](int scancode)
            { return EngineBindings::getInputManager()->isKeyPressed(static_cast<SDL_Scancode>(scancode)); }, "Check if a key was pressed this frame.");

      m.def("is_key_released", [](int scancode)
            { return EngineBindings::getInputManager()->isKeyReleased(static_cast<SDL_Scancode>(scancode)); }, "Check if a key was released this frame.");

      // Letters
      BIND_KEY("KEY_A", SDL_SCANCODE_A);
      BIND_KEY("KEY_B", SDL_SCANCODE_B);
      BIND_KEY("KEY_C", SDL_SCANCODE_C);
      BIND_KEY("KEY_D", SDL_SCANCODE_D);
      BIND_KEY("KEY_E", SDL_SCANCODE_E);
      BIND_KEY("KEY_F", SDL_SCANCODE_F);
      BIND_KEY("KEY_G", SDL_SCANCODE_G);
      BIND_KEY("KEY_H", SDL_SCANCODE_H);
      BIND_KEY("KEY_I", SDL_SCANCODE_I);
      BIND_KEY("KEY_J", SDL_SCANCODE_J);
      BIND_KEY("KEY_K", SDL_SCANCODE_K);
      BIND_KEY("KEY_L", SDL_SCANCODE_L);
      BIND_KEY("KEY_M", SDL_SCANCODE_M);
      BIND_KEY("KEY_N", SDL_SCANCODE_N);
      BIND_KEY("KEY_O", SDL_SCANCODE_O);
      BIND_KEY("KEY_P", SDL_SCANCODE_P);
      BIND_KEY("KEY_Q", SDL_SCANCODE_Q);
      BIND_KEY("KEY_R", SDL_SCANCODE_R);
      BIND_KEY("KEY_S", SDL_SCANCODE_S);
      BIND_KEY("KEY_T", SDL_SCANCODE_T);
      BIND_KEY("KEY_U", SDL_SCANCODE_U);
      BIND_KEY("KEY_V", SDL_SCANCODE_V);
      BIND_KEY("KEY_W", SDL_SCANCODE_W);
      BIND_KEY("KEY_X", SDL_SCANCODE_X);
      BIND_KEY("KEY_Y", SDL_SCANCODE_Y);
      BIND_KEY("KEY_Z", SDL_SCANCODE_Z);

      // Numbers
      BIND_KEY("KEY_0", SDL_SCANCODE_0);
      BIND_KEY("KEY_1", SDL_SCANCODE_1);
      BIND_KEY("KEY_2", SDL_SCANCODE_2);
      BIND_KEY("KEY_3", SDL_SCANCODE_3);
      BIND_KEY("KEY_4", SDL_SCANCODE_4);
      BIND_KEY("KEY_5", SDL_SCANCODE_5);
      BIND_KEY("KEY_6", SDL_SCANCODE_6);
      BIND_KEY("KEY_7", SDL_SCANCODE_7);
      BIND_KEY("KEY_8", SDL_SCANCODE_8);
      BIND_KEY("KEY_9", SDL_SCANCODE_9);

      // Modifiers & special
      BIND_KEY("KEY_SPACE", SDL_SCANCODE_SPACE);
      BIND_KEY("KEY_RETURN", SDL_SCANCODE_RETURN);
      BIND_KEY("KEY_ESCAPE", SDL_SCANCODE_ESCAPE);
      BIND_KEY("KEY_LSHIFT", SDL_SCANCODE_LSHIFT);
      BIND_KEY("KEY_RSHIFT", SDL_SCANCODE_RSHIFT);
      BIND_KEY("KEY_LCTRL", SDL_SCANCODE_LCTRL);
      BIND_KEY("KEY_RCTRL", SDL_SCANCODE_RCTRL);
      BIND_KEY("KEY_TAB", SDL_SCANCODE_TAB);

      // Arrow keys
      BIND_KEY("KEY_UP", SDL_SCANCODE_UP);
      BIND_KEY("KEY_DOWN", SDL_SCANCODE_DOWN);
      BIND_KEY("KEY_LEFT", SDL_SCANCODE_LEFT);
      BIND_KEY("KEY_RIGHT", SDL_SCANCODE_RIGHT);
}

#undef BIND_KEY