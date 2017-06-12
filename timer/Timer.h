//
// Created by morgan on 12.06.2017.
//

#ifndef ASYNC_TIMER_H
#define ASYNC_TIMER_H

#include "TimerDesc.h"
#include <memory>

namespace async {
    namespace timer {

        class Timer
        {
        public:
            Timer(TimerCbc callback, unsigned int dueTime, unsigned int period);
            ~Timer();

        private:
            std::shared_ptr<TimerDesc> timerDesc_;
        };
    }
}


#endif //ASYNC_TIMER_H
