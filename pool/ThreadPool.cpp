//
// Created by morgan on 07.06.2017.
//

#include <algorithm>
#include "ThreadPool.h"

namespace async {
    namespace pool {

        ThreadPool::ThreadPool(int concurrencyLevel)
        {
            for(auto i = 0; i < concurrencyLevel; ++i)
            {
                workers_.push_back(std::unique_ptr<Worker>(new Worker()));
            }
        }

        ThreadPool::~ThreadPool() {
        }

        bool ThreadPool::AddJob(Job job) {
            if (!workers_.size()) {
                job();
                return false;
            }

            std::unique_lock<std::mutex> lock(mutex_);
            auto &w = *std::min_element(workers_.begin(), workers_.end(),
                                        [](std::unique_ptr<Worker> &w1, std::unique_ptr<Worker> &w2) { return (*w1).JobsCount() < (*w2).JobsCount(); });
            return (*w).AddJob(job);
        }
    }
}