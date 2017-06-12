//
// Created by morgan on 12.06.2017.
//

#ifndef ASYNC_TIMERQUEUE_H
#define ASYNC_TIMERQUEUE_H

#include "../pool/CachedThreadPool.h"
#include "TimerDesc.h"

namespace async {
    namespace timer {

        class TimerQueue
        {
        public:
            TimerQueue();
            ~TimerQueue();

            void RegisterTimer(std::shared_ptr<TimerDesc> timer);
            void UnregisterTimer(std::shared_ptr<TimerDesc> timer);

        private:
            void TriggerTimers();
            void TimerQueueLoop();
            void SortTimers();
        private:
            std::vector<std::shared_ptr<TimerDesc>> timers_;
            std::mutex queueMutex_;
            std::atomic_bool running_;
            pool::CachedThreadPool pool_;
            std::condition_variable queueLoopUpdate_;
        };

        extern TimerQueue DefaultTimerQueue;
    }
}


#endif //ASYNC_TIMERQUEUE_H
