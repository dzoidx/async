//
// Created by morgan on 12.06.2017.
//

#include "TimerQueue.h"
#include "Timer.h"

namespace async {
    namespace timer {

        Timer::Timer(TimerCbc callback, unsigned int dueTime, unsigned int period)
        {
            timerDesc_.reset(new TimerDesc(callback, dueTime, period));
            DefaultTimerQueue.RegisterTimer(timerDesc_);
        }

        Timer::~Timer()
        {
            DefaultTimerQueue.UnregisterTimer(timerDesc_);
        }

    }
}
