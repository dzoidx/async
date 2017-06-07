//
// Created by morgan on 07.06.2017.
//

#include "pool/ThreadPool.h"
#include <iostream>
#include <thread>

std::mutex consoleLock;

void DummyJob2(int count)
{
    auto threadId = std::this_thread::get_id();
    std::unique_lock<std::mutex> lock(consoleLock);
    std::cout << "Job " << count << " on thread " << threadId << std::endl;
}

void DummyJob()
{
	auto threadId = std::this_thread::get_id();
	std::unique_lock<std::mutex> lock(consoleLock);
	std::cout << "Job on thread " << threadId << std::endl;
}

int main()
{
    async_pool::ThreadPool pool(20);

	for (auto i = 0; i < 100; ++i)
	{
		pool.AddJob(std::bind(DummyJob2, i));
	}

    //std::this_thread::sleep_for(std::chrono::milliseconds{2000});

	char c;
	std::cin >> c;

    return 0;
}

