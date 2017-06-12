//
// Created by morgan on 08.06.2017.
//

#ifndef ASYNC_TIMERDESC_H
#define ASYNC_TIMERDESC_H

#include <chrono>
#include <functional>
#include "../Definitions.h"

namespace async {
    namespace timer {

        using TimerCbc = std::function<void()>;

        class TimerDesc {
        public:
            TimerDesc(TimerCbc callback, unsigned int dueTime, unsigned int period);

            void Reset();

            SystemClock::time_point GetNext() const { return next_; }
            void Trigger();

        private:
            TimerCbc callback_;
            std::chrono::milliseconds period_;
            SystemClock::time_point next_;
        };
    }
}


#endif //ASYNC_TIMERDESC_H
