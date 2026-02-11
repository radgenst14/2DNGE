#include "Application.h"
#include "Window.h"

Application::Application()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return;
    }

    // Create a window with the specified title, width, and height
    auto title = new string("2DNGE");
    mWindow = std::make_unique<Window>(title, 800, 600);
    mIsRunning = true;
}

Application::~Application()
{
    mWindow.reset();
    SDL_Quit();
}

void Application::Run()
{
    while (mIsRunning)
    {
        Input(); // Handle input events via SDL_PollEvent
        Update();
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
    // Render application here
}