//
// Created by morgan on 07.06.2017.
//

#include "Worker.h"

namespace async_pool
{
    Worker::Worker()
    {
		thread_ = std::thread(&Worker::WorkerLoop, this);
    }

    Worker::~Worker()
    {
        running_ = false;
        jobEvent_.notify_all();
        thread_.join();
    }

    void Worker::AddJob(Job job)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        jobsQueue_.push(job);
        jobEvent_.notify_one();
    }

    void Worker::WorkerLoop() {

		bool running = running_;
        std::unique_lock<std::mutex> lock(mutex_);
        while (running_)
        {
            while(jobsQueue_.size())
            {
                auto& job = jobsQueue_.front();
                lock.unlock();
				job();
                lock.lock();
				jobsQueue_.pop();
            }
            jobEvent_.wait(lock, [&] { return jobsQueue_.size() || !(bool)running_;});
        }

    }

}