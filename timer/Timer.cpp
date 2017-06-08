//
// Created by morgan on 08.06.2017.
//

#include "Timer.h"

namespace async_timer
{

    Timer::Timer(TimecCbc callback, unsigned int dueTime, unsigned int period)
    {
        callback_ = callback;
        period_ = std::chrono::milliseconds(period);
        auto delay = std::chrono::milliseconds(dueTime);
        next_ = Clock::now() + delay;
    }

    void Timer::Reset()
    {
        next_ = Clock::now() + period_;
    }
}