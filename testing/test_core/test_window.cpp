#include <gtest/gtest.h>
#include "../../engine/core/Window.h"

TEST(WindowTest, ConstructionWithDefaultSize)
{
    Window window(new string("Test"));

    EXPECT_EQ(window.getWidth(), 0);
    EXPECT_EQ(window.getHeight(), 0);
    EXPECT_NE(window.getWindow(), nullptr);
}

TEST(WindowTest, ConstructionWithExplicitSize)
{
    Window window(new string("Sized"), 1280, 720);

    EXPECT_EQ(window.getWidth(), 1280);
    EXPECT_EQ(window.getHeight(), 720);
    EXPECT_NE(window.getWindow(), nullptr);
}

TEST(WindowTest, GetTitle)
{
    Window window(new string("My Title"));

    EXPECT_EQ(window.getTitle(), "My Title");
}

TEST(WindowTest, SetWidth)
{
    Window window(new string("Test"));
    window.setWidth(800);

    EXPECT_EQ(window.getWidth(), 800);
}

TEST(WindowTest, SetHeight)
{
    Window window(new string("Test"));
    window.setHeight(600);

    EXPECT_EQ(window.getHeight(), 600);
}

TEST(WindowTest, SetWidthAndHeight)
{
    Window window(new string("Test"));
    window.setWidth(1920);
    window.setHeight(1080);

    EXPECT_EQ(window.getWidth(), 1920);
    EXPECT_EQ(window.getHeight(), 1080);
}

TEST(WindowTest, SDLWindowIsCreated)
{
    Window window(new string("SDL Check"));
    SDL_Window *sdlWin = window.getWindow();

    ASSERT_NE(sdlWin, nullptr);

    // Verify SDL window title matches
    const char *title = SDL_GetWindowTitle(sdlWin);
    EXPECT_STREQ(title, "SDL Check");
}

TEST(WindowTest, SDLWindowHasCorrectSize)
{
    Window window(new string("Size Check"), 640, 480);

    int w, h;
    SDL_GetWindowSize(window.getWindow(), &w, &h);
    EXPECT_EQ(w, 640);
    EXPECT_EQ(h, 480);
}
