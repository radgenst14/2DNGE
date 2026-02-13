#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <string>
#include <SDL.h>

using namespace std;

/**
 * @class Window
 * @brief A class that represents a window in the application.
 *
 * This class provides methods to create, manage, and manipulate a window.
 */
class Window
{
public:
    Window(string *title, int width = 0, int height = 0);
    ~Window();

    /* Delete copy constructor and assignment operator */
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    /**
     * @brief Get the width and height of the window
     * @return The width and height of the window
     */
    int GetWidth() const { return mWidth; };
    int GetHeight() const { return mHeight; };

    /**
     * @brief  Set the width of the window
     * @param width
     */
    void SetWidth(int width) { mWidth = width; };
    void SetHeight(int height) { mHeight = height; };

    /**
     * @brief Get the title of the window
     * @return The title of the window
     */
    string GetTitle() const { return *mTitle; };

    /**
     * @brief Get the SDL_Window associated with this Window
     * @return
     */
    SDL_Window *GetWindow() const { return mWindow; };

private:
    int mWidth;     /**< The width of the window */
    int mHeight;    /**< The height of the window */
    string *mTitle; /**< The title of the window */

    SDL_Window *mWindow = nullptr; /**< The SDL window associated with this class */
};

#endif