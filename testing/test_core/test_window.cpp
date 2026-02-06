#include <gtest/gtest.h>
#include "../../engine/core/Window.h"

TEST(WindowTest, GettersAndSetters)
{
    Window window;

    // Test default values (assuming they are initialized to 0)
    EXPECT_EQ(window.GetWidth(), 0);
    EXPECT_EQ(window.GetHeight(), 0);

    // Set new values
    window.SetWidth(800);
    window.SetHeight(600);

    // Test if the getters return the correct values
    EXPECT_EQ(window.GetWidth(), 800);
    EXPECT_EQ(window.GetHeight(), 600);
}