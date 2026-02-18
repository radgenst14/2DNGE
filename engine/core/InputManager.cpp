#include "InputManager.h"

#include <cstring>
#include <SDL_keyboard.h>

InputManager::InputManager()
{
    // SDL owns this array and keeps it updated — grab the pointer once
    mCurrentKeyState = SDL_GetKeyboardState(nullptr);
}

InputManager::~InputManager()
{
}

void InputManager::clearBuffers()
{
    std::memset(mBufferedPresses, 0, SDL_NUM_SCANCODES);
    std::memset(mBufferedReleases, 0, SDL_NUM_SCANCODES);
}

void InputManager::processEvent(const SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN && !event.key.repeat)
    {
        mBufferedPresses[event.key.keysym.scancode] = 1;
    }
    else if (event.type == SDL_KEYUP)
    {
        mBufferedReleases[event.key.keysym.scancode] = 1;
    }
}

bool InputManager::isKeyDown(SDL_Scancode key) const
{
    return mCurrentKeyState && mCurrentKeyState[key];
}

bool InputManager::isKeyPressed(SDL_Scancode key) const
{
    return mBufferedPresses[key];
}

bool InputManager::isKeyReleased(SDL_Scancode key) const
{
    return mBufferedReleases[key];
}
