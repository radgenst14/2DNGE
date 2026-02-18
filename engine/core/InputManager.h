#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#pragma once

#include <SDL_scancode.h>
#include <SDL_events.h>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    /* Delete copy constructor and assignment operator */
    InputManager(const InputManager &) = delete;
    InputManager &operator=(const InputManager &) = delete;

    void clearBuffers();
    void processEvent(const SDL_Event &event);

    /** Keyboard state queries */
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyPressed(SDL_Scancode key) const;  // Just pressed this frame
    bool isKeyReleased(SDL_Scancode key) const; // Just released this frame

private:
    const Uint8 *mCurrentKeyState = nullptr;          ///< Current key state (owned by SDL)
    Uint8 mBufferedPresses[SDL_NUM_SCANCODES]{};      ///< Keys pressed at any point this frame
    Uint8 mBufferedReleases[SDL_NUM_SCANCODES]{};     ///< Keys released at any point this frame
};

#endif