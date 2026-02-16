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
    void tick();

    /** Get time since last tick() in seconds */
    double getDeltaTime() const { return mDeltaTime; }

    /** Get total time since timer creation in seconds */
    double getTotalTime() const { return mTotalTime; }

    /** Get current time in seconds (for sampling) */
    double getCurrentTime() const;

    /** Reset to zero */
    void reset();

private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint mStartTime;    /**< When the timer was created/reset */
    TimePoint mLastTickTime; /**< When tick() was last called */
    float mDeltaTime;        /**< Seconds between last two tick() calls */
    float mTotalTime;        /**< Total seconds since creation/reset */
};

#endif