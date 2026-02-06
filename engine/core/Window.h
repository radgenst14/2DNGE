#ifndef WINDOW_H
#define WINDOW_H

#pragma once

class Window
{
public:
    Window(int width = 0, int height = 0);
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

private:
    int mWidth;  ///< The width of the window
    int mHeight; ///< The height of the window
};

#endif