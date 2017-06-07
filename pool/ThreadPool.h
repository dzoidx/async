//
// Created by morgan on 07.06.2017.
//

#ifndef ASYNC_THREADPOOL_H
#define ASYNC_THREADPOOL_H

#include "Worker.h"

namespace async_pool
{
    using WorkersList = std::vector<Worker>;

    class ThreadPool
    {
    public:
        ThreadPool(int concurrencyLevel);
        ~ThreadPool();
        void AddJob(Job job);

    private:
        WorkersList workers_;
        std::mutex mutex_;
    };

}

#endif //ASYNC_THREADPOOL_H
