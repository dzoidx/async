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

namespace async_pool {

    using Job = std::function<void()>;
    class Worker
    {
    public:
        Worker();
        ~Worker();

        void AddJob(Job job);
        bool IsVacant() const { return vacant_; }
        int JobsCount() const { return jobsQueue_.size();}

    private:
        void WorkerLoop();
    private:
        std::queue<Job> jobsQueue_;
        std::atomic_bool vacant_;
        std::atomic_bool running_;
        std::thread thread_;
        std::mutex mutex_;
        std::condition_variable jobEvent_;
    };

}


#endif //ASYNC_WORKER_H
