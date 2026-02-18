#include <gtest/gtest.h>
#include <thread>
#include "../../engine/core/Timer.h"

TEST(TimerTest, InitialValuesAreZero)
{
    Timer timer;

    EXPECT_DOUBLE_EQ(timer.getDeltaTime(), 0.0);
    EXPECT_DOUBLE_EQ(timer.getTotalTime(), 0.0);
}

TEST(TimerTest, TickUpdatesDeltaTime)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.tick();

    EXPECT_GT(timer.getDeltaTime(), 0.0);
}

TEST(TimerTest, TickUpdatesTotalTime)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.tick();

    EXPECT_NEAR(timer.getTotalTime(), 0.1, 0.05);
}

TEST(TimerTest, MultipleTicks)
{
    Timer timer;

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.tick();
    double firstDelta = timer.getDeltaTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.tick();
    double secondDelta = timer.getDeltaTime();

    // Both deltas should be approximately 50ms
    EXPECT_NEAR(firstDelta, 0.05, 0.03);
    EXPECT_NEAR(secondDelta, 0.05, 0.03);

    // Total time should be the sum
    EXPECT_NEAR(timer.getTotalTime(), 0.1, 0.05);
}

TEST(TimerTest, GetCurrentTimeReturnsElapsed)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.tick();

    EXPECT_NEAR(timer.getCurrentTime(), timer.getTotalTime(), 0.01);
}

TEST(TimerTest, GetCurrentTimeWithoutTick)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // getCurrentTime should return elapsed time even without tick()
    EXPECT_GT(timer.getCurrentTime(), 0.0);
    // But total time should still be 0 (only updated by tick)
    EXPECT_DOUBLE_EQ(timer.getTotalTime(), 0.0);
}

TEST(TimerTest, ResetClearsAllValues)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.tick();

    EXPECT_GT(timer.getDeltaTime(), 0.0);
    EXPECT_GT(timer.getTotalTime(), 0.0);

    timer.reset();

    EXPECT_DOUBLE_EQ(timer.getDeltaTime(), 0.0);
    EXPECT_DOUBLE_EQ(timer.getTotalTime(), 0.0);
}

TEST(TimerTest, ResetAllowsFreshStart)
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.tick();
    timer.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    timer.tick();

    // After reset, total time should only reflect time since reset
    EXPECT_NEAR(timer.getTotalTime(), 0.05, 0.03);
}

TEST(TimerTest, DeltaTimeReflectsLastInterval)
{
    Timer timer;

    // Short interval
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    timer.tick();
    double shortDelta = timer.getDeltaTime();

    // Longer interval
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
    timer.tick();
    double longDelta = timer.getDeltaTime();

    // Longer interval should produce a larger delta
    EXPECT_GT(longDelta, shortDelta);
}
