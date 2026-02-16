#include "Application.h"
#include "Window.h"
#include "Timer.h"
#include "../renderer/Renderer.h"
#include "../ecs/EntityManager.h"
#include "../physics/PhysicsManager.h"

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

    if (!mWindow->GetWindow())
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    // Create a renderer for the window
    mRenderer = std::make_shared<Renderer>(mWindow.get());

    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return;
    }

    // Initialize the entity manager and physics manager
    mEntityManager = std::make_shared<EntityManager>();
    mPhysicsManager = std::make_shared<PhysicsManager>(mEntityManager.get());

    if (!mEntityManager)
    {
        SDL_Log("Failed to create EntityManager");
        return;
    }

    if (!mPhysicsManager)
    {
        SDL_Log("Failed to create PhysicsManager");
        return;
    }

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
    // Reset the timer so the first frame's delta is near zero
    mTimer.Reset();

    // Fixed timestep variables
    const double TICK_RATE = 60.0;           // 60 ticks per second
    const double FIXED_DT = 1.0 / TICK_RATE; // 0.01667 seconds per tick
    double accumulator = 0.0;

    while (mIsRunning)
    {
        // Update the timer and calculate the time since the last frame
        mTimer.Tick();
        double frameTime = mTimer.GetDeltaTime();
        accumulator += frameTime;

        // Handle input and update the application state at fixed intervals
        while (accumulator >= FIXED_DT)
        {
            Input();                       // Handle input events via SDL_PollEvent
            Update(mTimer.GetDeltaTime()); // Update application state (e.g., game logic, physics) at a fixed rate
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

void Application::Update(float dt)
{
    // Update the physics manager with the fixed delta time
    mPhysicsManager->update(dt);
}

void Application::Render()
{
}
