//
// Created by morgan on 12.06.2017.
//

#include <CachedThreadPool.h>
#include "ThreadPool.h"
#include "gtest/gtest.h"

TEST(pool_test, default_pool_test)
{
    const int concurrencyLvl = std::thread::hardware_concurrency();
    auto jobsDone = 0;
    std::mutex listLock;
    std::set<std::thread::id> threadIDs;
    {
        async::pool::ThreadPool pool(concurrencyLvl);

        for (auto i = 0; i < concurrencyLvl; ++i) {
            pool.AddJob([&] {
                std::unique_lock<std::mutex> lock(listLock);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                auto threadId = std::this_thread::get_id();
                ++jobsDone;
                threadIDs.insert(threadId);
            });
        }
    }

    EXPECT_EQ(concurrencyLvl, jobsDone);
    EXPECT_EQ(concurrencyLvl, threadIDs.size());

}

TEST(pool_test, cached_pool_spawn_workers_test)
{
    const int concurrencyLvl = std::thread::hardware_concurrency();
    std::atomic_int jobsDone;
    jobsDone.store(0);
    std::set<std::thread::id> threadIDs;
    int poolConcurrencyLvl;
    {
        async::pool::CachedThreadPool pool;
        for (auto i = 0; i < concurrencyLvl; ++i) {
            pool.AddJob([&] {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++jobsDone;
                threadIDs.insert(std::this_thread::get_id());
            });
        }
    }

    poolConcurrencyLvl = threadIDs.size();
    EXPECT_EQ(concurrencyLvl, jobsDone);
    EXPECT_EQ(concurrencyLvl, poolConcurrencyLvl);
}

TEST(pool_test, cached_pool_destroy_workers_test)
{
    const int concurrencyLvl = std::thread::hardware_concurrency();
    const int hConcurrencyLvl = std::max(1, concurrencyLvl / 2);
    std::mutex lok;
    std::atomic_int jobsDone;
    std::set<std::thread::id> threadIDs;
    jobsDone.store(0);
    int poolConcurrencyLvl;
    {
        async::pool::CachedThreadPool pool;
        for (auto i = 0; i < concurrencyLvl; ++i) {
            pool.AddJob([&] {
                std::unique_lock<std::mutex> lock(lok);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++jobsDone;
                threadIDs.insert(std::this_thread::get_id());
            });
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(async::pool::WorkerDestroyDelay + 100));

        poolConcurrencyLvl = pool.GetConcurrencyLevel();

        EXPECT_EQ(poolConcurrencyLvl, threadIDs.size());
        EXPECT_EQ(poolConcurrencyLvl, concurrencyLvl);
        EXPECT_EQ(jobsDone, concurrencyLvl);

        jobsDone = 0;
        threadIDs.clear();

        for (auto i = 0; i < hConcurrencyLvl; ++i) {
            pool.AddJob([&] {
                std::unique_lock<std::mutex> lock(lok);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                ++jobsDone;
                threadIDs.insert(std::this_thread::get_id());
            });
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10 * hConcurrencyLvl * 2));

        poolConcurrencyLvl = pool.GetConcurrencyLevel();
    }

    EXPECT_EQ(poolConcurrencyLvl, threadIDs.size());
    EXPECT_EQ(poolConcurrencyLvl, hConcurrencyLvl);
    EXPECT_EQ(jobsDone, hConcurrencyLvl);
}