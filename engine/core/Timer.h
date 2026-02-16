#ifndef TIMER_H
#define TIMER_H

#pragma once
#include <chrono>

class Timer
{
public:
    Timer();

    Timer(const Timer &) = delete;
    Timer &operator=(const Timer &) = delete;

    /** Call this once per frame to update delta time */
    void Tick();

    /** Get time since last Tick() in seconds */
    double GetDeltaTime() const { return mDeltaTime; }

    /** Get total time since timer creation in seconds */
    double GetTotalTime() const { return mTotalTime; }

    /** Get current time in seconds (for sampling) */
    double GetCurrentTime() const;

    /** Reset to zero */
    void Reset();

private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint mStartTime;    /**< When the timer was created/reset */
    TimePoint mLastTickTime; /**< When Tick() was last called */
    float mDeltaTime;        /**< Seconds between last two Tick() calls */
    float mTotalTime;        /**< Total seconds since creation/reset */
};

#endif