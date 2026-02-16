#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

#include <memory>
#include <SDL.h>
#include "Timer.h"

class Window;
class Renderer;
class SceneManager;

/**
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

    /* @brief Delete copy constructor and assignment operator */
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    /** @brief Runs the main application loop, handling events and updating the application state. */
    void run();

    /** @brief  Methods for handling input, updating the application state, and rendering the application. */
    void handleInput();
    void update(float dt);
    void render();

private:
    bool mIsRunning = false;
    Timer mTimer; /**< A timer for managing frame timing */

    std::shared_ptr<Window> mWindow = nullptr;   /**< The main application window */
    std::shared_ptr<Renderer> mRenderer;         /**< The renderer for drawing graphics */
    std::shared_ptr<SceneManager> mSceneManager; /**< The scene manager for managing game scenes */
};

#endif