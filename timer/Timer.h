//
// Created by morgan on 08.06.2017.
//

#ifndef ASYNC_TIMER_H
#define ASYNC_TIMER_H

#include <chrono>
#include <functional>

namespace async_timer {

    using TimecCbc = std::function<void()>;
    using Clock = std::chrono::high_resolution_clock;

    class Timer
    {
    public:
        Timer(TimecCbc callback, unsigned int dueTime, unsigned int period);
        void Reset();
        Clock::time_point GetNext() const { return next_; }
    private:
        TimecCbc callback_;
        std::chrono::milliseconds period_;
        Clock::time_point next_;
    };

}


#endif //ASYNC_TIMER_H
