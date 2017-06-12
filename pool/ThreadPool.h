//
// Created by morgan on 07.06.2017.
//

#ifndef ASYNC_THREADPOOL_H
#define ASYNC_THREADPOOL_H

#include "Worker.h"

namespace async {
    namespace pool {
        using WorkersList = std::vector<std::unique_ptr<Worker>>;

        class ThreadPool {
        public:
            ThreadPool(int concurrencyLevel);

            virtual ~ThreadPool();

            virtual bool AddJob(Job job);
            int GetConcurrencyLevel() { return workers_.size(); }

        protected:
            WorkersList workers_;
            std::mutex mutex_;
        };
    }
}

#endif //ASYNC_THREADPOOL_H
