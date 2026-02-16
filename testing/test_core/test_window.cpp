#include <gtest/gtest.h>
#include "../../engine/core/Window.h"

TEST(WindowTest, GettersAndSetters)
{
    Window window(new string("Test Window"));

    // Test default values (assuming they are initialized to 0)
    EXPECT_EQ(window.getWidth(), 0);
    EXPECT_EQ(window.getHeight(), 0);
    EXPECT_NE(window.getWindow(), nullptr);

    // Test title getter
    EXPECT_EQ(window.getTitle(), "Test Window");

    // Set new values
    window.setWidth(800);
    window.setHeight(600);

    // Test if the getters return the correct values
    EXPECT_EQ(window.getWidth(), 800);
    EXPECT_EQ(window.getHeight(), 600);
}