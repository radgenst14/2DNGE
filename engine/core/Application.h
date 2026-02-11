#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

#include <memory>
#include <SDL.h>

class Window;

/*
 * @class Application
 * @brief A class that represents the main application.
 *
 * This class provides methods to initialize, run, and manage the application.
 */
class Application
{
public:
    Application();
    ~Application();

    // Delete copy constructor and assignment operator
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    /// @brief Runs the main application loop, handling events and updating the application state.
    void Run();

    /// @brief  Methods for handling input, updating the application state, and rendering the application.
    void Input();
    void Update();
    void Render();

private:
    bool mIsRunning = false;
    std::unique_ptr<Window> mWindow;
};

#endif