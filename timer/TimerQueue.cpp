//
// Created by morgan on 12.06.2017.
//

#include "TimerQueue.h"
#include <algorithm>

namespace async {
    namespace timer {

        TimerQueue DefaultTimerQueue;

        TimerQueue::TimerQueue()
        {
            running_ = true;
            pool_.AddJob(std::bind(&TimerQueue::TimerQueueLoop, this));
        }

        TimerQueue::~TimerQueue()
        {
            running_ = false;
            queueLoopUpdate_.notify_all();
        }

        void TimerQueue::TimerQueueLoop()
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            while (running_)
            {
                if(!timers_.size())
                {
                    queueLoopUpdate_.wait(lock);
                }
                else
                {
                    auto tn = SystemClock::now();
                    auto nextWakeup = (*timers_.front()).GetNext() - tn;
                    if(nextWakeup > nextWakeup.zero())
                        queueLoopUpdate_.wait_until(lock, (*timers_.front()).GetNext());
                }

                if(running_) {
                    TriggerTimers();
                    SortTimers();
                }
            }
        }

        void TimerQueue::RegisterTimer(std::shared_ptr<TimerDesc> timer)
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            auto exists = std::find_if(timers_.begin(), timers_.end(), [&] (std::shared_ptr<TimerDesc>& t) { return t.get() == timer.get(); }) != timers_.end();
            if(exists)
                return;

            timers_.push_back(timer);
            SortTimers();
            queueLoopUpdate_.notify_one();
        }

        void TimerQueue::UnregisterTimer(std::shared_ptr<TimerDesc> timer)
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            auto pos = std::find_if(timers_.begin(), timers_.end(), [&] (std::shared_ptr<TimerDesc>& t) { return t.get() == timer.get(); });
            if(pos == timers_.end())
                return;

            timers_.erase(pos);
            SortTimers();
            queueLoopUpdate_.notify_one();
        }

        void TimerQueue::SortTimers()
        {
            auto t = Clock::now();
            std::sort(timers_.begin(), timers_.end(), [&] (std::shared_ptr<TimerDesc>& t1, std::shared_ptr<TimerDesc>& t2)
            {
                auto t1d = (*t1).GetNext() - t;
                auto t2d = (*t2).GetNext() - t;
                return t1d < t2d;
            });
        }

        void TimerQueue::TriggerTimers()
        {
            auto tn = SystemClock::now();
            for(auto& t : timers_)
            {
                auto d = (*t).GetNext() - tn;
                if(d <= d.zero()) {
                    t->Reset();
                    pool_.AddJob(std::bind(&TimerDesc::Trigger, t.get()));
                }
            }
        }
    }
}