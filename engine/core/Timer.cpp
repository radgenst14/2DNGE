#include "Timer.h"

Timer::Timer()
    : mDeltaTime(0.0), mTotalTime(0.0)
{
    mStartTime = Clock::now();
    mLastTickTime = mStartTime;
}

void Timer::Tick()
{
    TimePoint currentTime = Clock::now();

    // Calculate delta in seconds
    std::chrono::duration<double> delta = currentTime - mLastTickTime;
    mDeltaTime = delta.count();

    // Update total time
    std::chrono::duration<double> total = currentTime - mStartTime;
    mTotalTime = total.count();

    mLastTickTime = currentTime;
}

double Timer::GetCurrentTime() const
{
    TimePoint currentTime = Clock::now();
    std::chrono::duration<double> elapsed = currentTime - mStartTime;
    return elapsed.count();
}

void Timer::Reset()
{
    mStartTime = Clock::now();
    mLastTickTime = mStartTime;
    mDeltaTime = 0.0;
    mTotalTime = 0.0;
}