#include "InputManager.h"

#include <cstring>
#include <SDL_keyboard.h>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::update()
{
    // Snapshot current key state to previous
    if (mCurrentKeyState)
    {
        std::memcpy(mPreviousKeyState, mCurrentKeyState, SDL_NUM_SCANCODES);
    }

    // SDL owns this array — just grab the pointer
    mCurrentKeyState = SDL_GetKeyboardState(nullptr);
}

bool InputManager::isKeyDown(SDL_Scancode key) const
{
    return mCurrentKeyState && mCurrentKeyState[key];
}

bool InputManager::isKeyPressed(SDL_Scancode key) const
{
    return mCurrentKeyState && mCurrentKeyState[key] && !mPreviousKeyState[key];
}

bool InputManager::isKeyReleased(SDL_Scancode key) const
{
    return mCurrentKeyState && !mCurrentKeyState[key] && mPreviousKeyState[key];
}