//
// Created by morgan on 08.06.2017.
//

#include "TimerDesc.h"

namespace async {
    namespace timer {

        TimerDesc::TimerDesc(TimerCbc callback, unsigned int dueTime, unsigned int period) {
            callback_ = callback;
            period_ = std::chrono::milliseconds(period);
            auto delay = std::chrono::milliseconds(dueTime);
            next_ = Clock::now() + delay;
        }

        void TimerDesc::Reset() {
            next_ = Clock::now() + period_;
        }

        void TimerDesc::Trigger()
        {
            callback_();
            Reset();
        }
    }
}