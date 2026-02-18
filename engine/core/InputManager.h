#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#pragma once

#include <SDL_scancode.h>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    /* Delete copy constructor and assignment operator */
    InputManager(const InputManager &) = delete;
    InputManager &operator=(const InputManager &) = delete;

    void update();

    /** Keyboard state queries */
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyPressed(SDL_Scancode key) const;  // Just pressed this frame
    bool isKeyReleased(SDL_Scancode key) const; // Just released this frame

private:
    const Uint8 *mCurrentKeyState = nullptr;      ///< Current key state (owned by SDL)
    Uint8 mPreviousKeyState[SDL_NUM_SCANCODES]{}; ///< Previous key state (owned by InputManager)
};

#endif