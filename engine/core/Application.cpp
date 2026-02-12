#include "Application.h"
#include "Window.h"
#include "../renderer/Renderer.h"
#include "Timer.h"

Application::Application()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());

        return;
    }

    // Create a window with the specified title, width, and height
    auto title = new string("2DNGE");
    mWindow = std::shared_ptr<Window>(new Window(title, 800, 600));

    // Check if the window was created successfully
    if (!mWindow->GetWindow())
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    // Create a renderer for the window
    mRenderer = std::make_shared<Renderer>(mWindow.get());

    // Set the application as running
    mIsRunning = true;
}

Application::~Application()
{
    mWindow.reset();
    SDL_Quit();
}

void Application::Run()
{
    Timer timer;

    // Fixed timestep variables
    const double TICK_RATE = 60.0;           // 60 ticks per second
    const double FIXED_DT = 1.0 / TICK_RATE; // 0.01667 seconds per tick
    double accumulator = 0.0;

    while (mIsRunning)
    {
        // Update the timer and calculate the time since the last frame
        mTimer->Tick();
        double frameTime = timer.GetDeltaTime();
        accumulator += frameTime;

        // Handle input and update the application state at fixed intervals
        while (accumulator >= FIXED_DT)
        {
            Input(); // Handle input events via SDL_PollEvent
            Update();
            accumulator -= FIXED_DT;
        }

        // Render the application
        Render();
    }
}

void Application::Input()
{
    SDL_Event event;

    // Poll for events and handle them
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            mIsRunning = false;
        }
    }
}

void Application::Update()
{
    // Update application state here
}

void Application::Render()
{
}
