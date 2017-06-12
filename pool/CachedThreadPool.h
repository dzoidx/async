//
// Created by morgan on 12.06.2017.
//

#ifndef ASYNC_CACHEDTHREADPOOL_H
#define ASYNC_CACHEDTHREADPOOL_H

#include "ThreadPool.h"

namespace async {
    namespace pool {

        const int WorkerDestroyDelay = 5000; // milliseconds
        class CachedThreadPool : public ThreadPool
        {
        public:
            CachedThreadPool() : ThreadPool(0) {}
            bool AddJob(Job job) override;

        private:
            void CleanCache(WorkersList::iterator end);
        };
    }
}


#endif //ASYNC_CACHEDTHREADPOOL_H
