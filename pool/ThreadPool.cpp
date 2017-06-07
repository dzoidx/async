//
// Created by morgan on 07.06.2017.
//

#include <algorithm>
#include "ThreadPool.h"

namespace async_pool
{
    ThreadPool::ThreadPool(int concurrencyLevel)
    : workers_(concurrencyLevel)
    {
    }

    ThreadPool::~ThreadPool()
    {
    }

    void ThreadPool::AddJob(Job job)
    {
        if(!workers_.size()) {
            job();
            return;
        }

        std::unique_lock<std::mutex> lock(mutex_);
        auto& w = *std::min_element(workers_.begin(), workers_.end(), [] (Worker& w1, Worker& w2) { return w1.JobsCount() < w2.JobsCount(); });
        w.AddJob(job);
    }
}