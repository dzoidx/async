//
// Created by morgan on 07.06.2017.
//

#include "Worker.h"

namespace async {
    namespace pool {

        Worker::Worker() {
            running_ = true;
            lastJobFinishTime_ = Clock::now();
            thread_ = std::thread(&Worker::WorkerLoop, this);
        }

        Worker::~Worker() {
            running_ = false;
            jobEvent_.notify_all();
            thread_.join();
        }

        bool Worker::AddJob(Job job) {
            std::unique_lock<std::mutex> lock(mutex_);
            if(!running_)
            {
                return false;
            }
            jobsQueue_.push(job);
            jobEvent_.notify_one();
            return true;
        }

        void Worker::WorkerLoop() {

            bool running = running_;
            std::unique_lock<std::mutex> lock(mutex_);
            while (running_) {
                while (jobsQueue_.size()) {
                    auto &job = jobsQueue_.front();
                    lock.unlock();
                    job();
                    lock.lock();
                    lastJobFinishTime_ = Clock::now();
                    jobsQueue_.pop();
                }
                jobEvent_.wait(lock, [&] { return jobsQueue_.size() || !(bool) running_; });
            }
            // finish jobs on destroy
            while (jobsQueue_.size())
            {
                auto &job = jobsQueue_.front();
                job();
                jobsQueue_.pop();
            }

        }
    }
}