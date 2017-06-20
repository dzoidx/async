# async - minimalistic threading extension for C++11 and above #


### What is async? ###

It's small lib which adds supports of thread pools and timers.

It implements:

* Simple thread pool - hard limited pool of workers
* Cached thread pool - amount of workers grows and fades corresponding to the load
* Timer queue - convenient way to do things periodically

### Using ThreadPool ###


```
#!c++

std::mutex consoleLock;
auto start = async::Clock::now();

void DummyJob(int count)
{
    auto threadId = std::this_thread::get_id();
    std::unique_lock<std::mutex> lock(consoleLock);
    std::cout << "Job " << count
              << " on thread " << threadId
              << std::endl;
}

int main()
{
    {
        int workersCount = std::thread::hardware_concurrency();
        int jobsCount = workersCount * 2;
        async::pool::ThreadPool pool(workersCount);

        for (auto i = 0; i < jobsCount; ++i)
        {
            pool.AddJob(std::bind(DummyJob, i));
        }
    }

    system("pause");
}

output:
WorkersCount = 8
JobsCount = 16
Job 0 on thread 3
Job 1 on thread 4
Job 9 on thread 4
Job 3 on thread 6
Job 11 on thread 6
Job 4 on thread 7
Job 6 on thread 9
Job 14 on thread 9
Job 8 on thread 3
Job 2 on thread 5
Job 5 on thread 8
Job 12 on thread 7
Job 7 on thread 10
Job 15 on thread 10
Job 13 on thread 8
Job 10 on thread 5

```

### Using Timer ###

```
#!c++
std::mutex consoleLock;
auto start = async::Clock::now();

void DummyJob(int count)
{
    auto enterDTime = async::Clock::now() - start;
    auto threadId = std::this_thread::get_id();
    std::unique_lock<std::mutex> lock(consoleLock);
    std::cout << "Job " << count
              << " on thread " << threadId
              << " delta " << std::chrono::duration_cast<std::chrono::milliseconds>(enterDTime).count()
              << std::endl;
}

int main()
{
    async::timer::Timer t1(std::bind(DummyJob, 1), 500, 100); // wait 500 ms and then fire each 100 ms
    async::timer::Timer t2(std::bind(DummyJob, 2), 0, 1500); // nowait, fire each 1,5 sec

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

output:
Job 2 on thread 3 delta 0
Job 1 on thread 2 delta 500
Job 1 on thread 2 delta 600
Job 1 on thread 2 delta 700
Job 1 on thread 2 delta 801
Job 1 on thread 2 delta 902
Job 1 on thread 2 delta 1003
Job 1 on thread 2 delta 1105
Job 1 on thread 2 delta 1206
Job 1 on thread 3 delta 1307
Job 1 on thread 2 delta 1408
Job 2 on thread 2 delta 1501
Job 1 on thread 2 delta 1509
Job 1 on thread 2 delta 1609
Job 1 on thread 3 delta 1710
Job 1 on thread 3 delta 1812
Job 1 on thread 2 delta 1913
Job 1 on thread 2 delta 2013
Job 1 on thread 2 delta 2114
Job 1 on thread 2 delta 2214
Job 1 on thread 3 delta 2316
Job 1 on thread 2 delta 2416
Job 1 on thread 2 delta 2516
Job 1 on thread 2 delta 2616
Job 1 on thread 2 delta 2717
Job 1 on thread 2 delta 2817
Job 1 on thread 3 delta 2919
```
### Using CachedThreadPool ###

```
#!c++
std::mutex consoleLock;
auto start = async::Clock::now();

void DummyJob(int count)
{
    auto threadId = std::this_thread::get_id();
    std::unique_lock<std::mutex> lock(consoleLock);
    std::cout << "Job " << count
              << " on thread " << threadId
              << std::endl;
}

int main()
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

output:
LogicalCoresCount = 8
JobsCountFirst = 16
JobsCountSecond = 4
Job 0 on thread 3
Job 1 on thread 4
Job 2 on thread 5
Job 3 on thread 6
Job 4 on thread 7
Job 7 on thread 10
Job 6 on thread 9
Job 5 on thread 8
Pool concurrency level is 15
Job 15 on thread 3
Job 8 on thread 11
Job 9 on thread 12
Job 10 on thread 13
Job 11 on thread 14
Job 12 on thread 15
Job 13 on thread 16
Job 14 on thread 17
Job 0 on thread 3
Pool concurrency level is 4
Job 1 on thread 18
Job 2 on thread 19
Job 3 on thread 20
```