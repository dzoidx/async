//
// Created by morgan on 07.06.2017.
//

#include "Definitions.h"
#include "pool/ThreadPool.h"
#include "pool/CachedThreadPool.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "timer/Timer.h"

std::mutex consoleLock;
auto start = async::Clock::now();

void DummyJob(int count)
{
    auto enterDTime = async::Clock::now() - start;
    auto threadId = std::this_thread::get_id();
    std::unique_lock<std::mutex> lock(consoleLock);

    std::cout << "Job " << count
              << " on thread " << threadId
              //<< " delta " << std::chrono::duration_cast<std::chrono::milliseconds>(enterDTime).count()
              << std::endl;
}

void TestThreadPool()
{
    int workersCount = std::thread::hardware_concurrency();
    int jobsCount = workersCount * 2;
    async::pool::ThreadPool pool(workersCount);

    std::cout << "WorkersCount = " << workersCount << std::endl;
    std::cout << "JobsCount = " << jobsCount << std::endl;

    for (auto i = 0; i < jobsCount; ++i)
    {
        pool.AddJob(std::bind(DummyJob, i));
    }
}

void TestCachedThreadPool()
{
    int logicalCoresCount = std::thread::hardware_concurrency();
    int jobsCountFirst = logicalCoresCount * 2;
    int jobsCountSecond = logicalCoresCount / 2;
    async::pool::CachedThreadPool pool;

    std::cout << "LogicalCoresCount = " << logicalCoresCount << std::endl;
    std::cout << "JobsCountFirst = " << jobsCountFirst << std::endl;
    std::cout << "JobsCountSecond = " << jobsCountSecond << std::endl;

    for (auto i = 0; i < jobsCountFirst; ++i)
    {
        pool.AddJob(std::bind(DummyJob, i));
    }

    {
        std::unique_lock<std::mutex> lock(consoleLock);
        std::cout << "Pool concurrency level is " << pool.GetConcurrencyLevel() << std::endl;
    }

    //ensure some workers are expired
    std::this_thread::sleep_for(std::chrono::milliseconds(async::pool::WorkerDestroyDelay + 100));

    for (auto i = 0; i < jobsCountSecond; ++i)
    {
        pool.AddJob(std::bind(DummyJob, i));
    }

    {
        std::unique_lock<std::mutex> lock(consoleLock);
        std::cout << "Pool concurrency level is " << pool.GetConcurrencyLevel() << std::endl;
    }
}

void TestTimers()
{
    async::timer::Timer t1(std::bind(DummyJob, 1), 500, 100); // wait 500 ms and then fire each 100 ms
    async::timer::Timer t2(std::bind(DummyJob, 2), 0, 1500); // nowait, fire each 1,5 sec

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int main()
{
    TestCachedThreadPool();

    system("pause");
}

