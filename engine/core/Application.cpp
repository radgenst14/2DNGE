#include "Application.h"
#include "Window.h"
#include "Timer.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "../scripting/ScriptableScene.h"
#include "ScriptingConfig.h"

#include <string>

Application::Application()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());

        return;
    }

    // Create a window with the specified title, width, and height
    mWindow = std::make_shared<Window>("2DNGE", 800, 600);

    if (!mWindow->getWindow())
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    // Initialize the scene manager
    mSceneManager = std::make_shared<SceneManager>();

    if (!mSceneManager)
    {
        SDL_Log("Failed to create scene manager");
        return;
    }

    // Initialize the input manager
    mInputManager = std::make_shared<InputManager>();

    if (!mInputManager)
    {
        SDL_Log("Failed to create input manager: %s", SDL_GetError());
        return;
    }

    // Set the initial scene
    std::string scriptPath = std::string(PROJECT_ROOT) + "/game/scenes/MainScene.py";
    mSceneManager->setScene(std::make_unique<ScriptableScene>(scriptPath, mWindow.get()));

    // Set the application as running
    mIsRunning = true;
}

Application::~Application()
{
    // Destroy all SDL-dependent objects before calling SDL_Quit
    mSceneManager.reset();
    mInputManager.reset();
    mWindow.reset();
    SDL_Quit();
}

void Application::run()
{
    // Reset the timer so the first frame's delta is near zero
    mTimer.reset();

    // Fixed timestep variables
    const double TICK_RATE = 60.0;           // 60 ticks per second
    const double FIXED_DT = 1.0 / TICK_RATE; // 0.01667 seconds per tick
    double accumulator = 0.0;

    while (mIsRunning)
    {
        // Clear previous frame's input buffers
        mInputManager->clearBuffers();

        // Update the timer and calculate the time since the last frame
        mTimer.tick();
        double frameTime = mTimer.getDeltaTime();
        accumulator += frameTime;

        // Poll events and update key state
        handleInput();

        // Update the application state at fixed intervals
        while (accumulator >= FIXED_DT)
        {
            update(FIXED_DT); // Update application state (e.g., game logic, physics) at a fixed rate
            accumulator -= FIXED_DT;
        }

        // Render the application
        render();
    }
}

void Application::handleInput()
{
    SDL_Event event;

    // Poll for events and handle them
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            mIsRunning = false;
        }

        mInputManager->processEvent(event);
    }
}

void Application::update(float dt)
{
    mSceneManager->getActiveScene()->update(dt);
}

void Application::render()
{
    mSceneManager->getActiveScene()->render();
}
