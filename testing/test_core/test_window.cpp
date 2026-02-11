#include <gtest/gtest.h>
#include "../../engine/core/Window.h"

TEST(WindowTest, GettersAndSetters)
{
    Window window = Window(new string("Test Window"));

    // Test default values (assuming they are initialized to 0)
    EXPECT_EQ(window.GetWidth(), 0);
    EXPECT_EQ(window.GetHeight(), 0);
    EXPECT_EQ(window.GetWindow(), nullptr);
    EXPECT_EQ(window.GetSurface(), nullptr);

    // Test title getter
    EXPECT_EQ(window.GetTitle(), "Test Window");

    // Set new values
    window.SetWidth(800);
    window.SetHeight(600);

    // Test if the getters return the correct values
    EXPECT_EQ(window.GetWidth(), 800);
    EXPECT_EQ(window.GetHeight(), 600);
}