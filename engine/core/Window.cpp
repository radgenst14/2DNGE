#include "Window.h"

Window::Window(const string &title, int width, int height)
    : mTitle(title), mWidth(width), mHeight(height)
{
    mWindow = SDL_CreateWindow(
        mTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        mWidth,
        mHeight,
        SDL_WINDOW_SHOWN);

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }
}

Window::~Window()
{
    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
}