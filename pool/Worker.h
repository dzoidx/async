//
// Created by morgan on 07.06.2017.
//

#ifndef ASYNC_WORKER_H
#define ASYNC_WORKER_H

#include <atomic>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <condition_variable>
#include <chrono>
#include "../Definitions.h"

namespace async {
    namespace pool {

        using Job = std::function<void()>;

        class Worker {
        public:
            Worker();

            ~Worker();

            void AddJob(Job job);

            int JobsCount() const { return jobsQueue_.size(); }
            Clock::time_point GetLastJobTime() const { return lastJobFinishTime_; }

        private:
            void WorkerLoop();

        private:
            std::queue<Job> jobsQueue_;
            std::atomic_bool running_;
            std::thread thread_;
            std::mutex mutex_;
            std::condition_variable jobEvent_;
            Clock::time_point lastJobFinishTime_;
        };
    }
}


#endif //ASYNC_WORKER_H
