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

    // Delete copy constructor and assignment operator
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    /*
     * Width and Height getters
     */
    int GetWidth() const { return mWidth; };
    int GetHeight() const { return mHeight; };

    /*
     * Width and Height setters
     */
    void SetWidth(int width) { mWidth = width; };
    void SetHeight(int height) { mHeight = height; };

    /*
     * Title getter
     */
    string GetTitle() const { return *mTitle; };

    /*
     * SDL Window and Surface getters
     */
    SDL_Window *GetWindow() const { return mWindow; };
    SDL_Surface *GetSurface() const { return mSurface; };

    /*
     * Set the surface of the window, which can be used for rendering operations.
     */
    void SetSurface(SDL_Surface *surface) { mSurface = surface; };

private:
    int mWidth;     ///< The width of the window
    int mHeight;    ///< The height of the window
    string *mTitle; ///< The title of the window

    SDL_Window *mWindow = nullptr;   ///< The SDL window associated with this class
    SDL_Surface *mSurface = nullptr; ///< The surface associated with the SDL window
};

#endif