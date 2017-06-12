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

void DummyJob2(int count)
{
    auto enterDTime = async::Clock::now() - start;
    auto threadId = std::this_thread::get_id();
    std::unique_lock<std::mutex> lock(consoleLock);
    std::cout << "Job " << count
              << " on thread " << threadId
              << " delta " << std::chrono::duration_cast<std::chrono::milliseconds>(enterDTime).count()
              << std::endl;
}

void DummyJob()
{
	auto threadId = std::this_thread::get_id();
	std::unique_lock<std::mutex> lock(consoleLock);
	std::cout << "Job on thread " << threadId << std::endl;
}

void TestThreadPool()
{
    async::pool::ThreadPool pool(8);

    for (auto i = 0; i < 10; ++i)
    {
        pool.AddJob(std::bind(DummyJob2, i));
    }
}

void TestCachedThreadPool()
{
    async::pool::CachedThreadPool pool;

    for (auto i = 0; i < 10; ++i)
    {
        pool.AddJob(std::bind(DummyJob2, i));
    }

    {
        std::unique_lock<std::mutex> lock(consoleLock);
        std::cout << "Pool concurrency level is " << pool.GetConcurrencyLevel() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (auto i = 0; i < 5; ++i)
    {
        pool.AddJob(std::bind(DummyJob2, i));
    }

    {
        std::unique_lock<std::mutex> lock(consoleLock);
        std::cout << "Pool concurrency level is " << pool.GetConcurrencyLevel() << std::endl;
    }
}

void TestTimers()
{
    async::timer::Timer t1(std::bind(DummyJob2, 1), 500, 800);
    async::timer::Timer t2(std::bind(DummyJob2, 2), 0, 1500);
    //async::timer::Timer t3(std::bind(DummyJob2, 3), 100, 100);

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}

int main()
{
    TestTimers();

	char c;
	std::cin >> c;

    return 0;
}

