#include <gtest/gtest.h>
#include "../../engine/core/Timer.h"

TEST(TimerTest, GettersAndSetters)
{
    Timer timer;

    // Test initial values
    EXPECT_DOUBLE_EQ(timer.getDeltaTime(), 0.0);
    EXPECT_DOUBLE_EQ(timer.getTotalTime(), 0.0);

    // Simulate some time passing
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.tick();

    // Test that delta time is greater than zero and total time is approximately 0.1 seconds
    EXPECT_GT(timer.getDeltaTime(), 0.0);
    EXPECT_NEAR(timer.getTotalTime(), 0.1, 0.05);

    // Test getCurrentTime returns a value close to total time
    EXPECT_NEAR(timer.getCurrentTime(), timer.getTotalTime(), 0.01);

    // Reset the timer and test that values are reset to zero
    timer.reset();
    EXPECT_DOUBLE_EQ(timer.getDeltaTime(), 0.0);
    EXPECT_DOUBLE_EQ(timer.getTotalTime(), 0.0);
}