//
// Created by morgan on 12.06.2017.
//

#include <algorithm>
#include "CachedThreadPool.h"

namespace async {
    namespace pool {

        void CachedThreadPool::AddJob(Job job)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            auto w = std::find_if(workers_.begin(), workers_.end(), [] (std::unique_ptr<Worker> &w) { return !(*w).JobsCount(); });
            if(w == workers_.end())
            {
                workers_.push_back(std::unique_ptr<Worker>(new Worker()));
                w = workers_.end() - 1;
            }
            else
            {
                CleanCache(w);
            }
            (**w).AddJob(job);
        }

        void CachedThreadPool::CleanCache(WorkersList::iterator end)
        {
            auto delCount = 0;
            for (auto it = workers_.end() - 1; it > end; --it) {
                auto lastJobD = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - (**it).GetLastJobTime()).count();
                if(lastJobD < WorkerDestroyDelay)
                {
                    break;
                }

                ++delCount;
            }

            if(!delCount)
                return;

            workers_.resize(workers_.size() - delCount);
        }
    }
}