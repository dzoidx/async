//
// Created by morgan on 12.06.2017.
//

#include "Timer.h"
#include "gtest/gtest.h"
#include <thread>

TEST(timer_test, timer_cbk_test)
{
    std::map<int, int> averageCallbackDelay { {1, 0}, {2, 0}};
    auto timerCbk = [&] (int timerId) {
        ++averageCallbackDelay[timerId];
    };
    int timer1ValidCountMin = 25;
    int timer1ValidCountMax = 26;
    int timer2ValidCountMin = 1;
    int timer2ValidCountMax = 2;
    {
        async::timer::Timer t1(std::bind(timerCbk, 1), 500, 100);
        async::timer::Timer t2(std::bind(timerCbk, 2), 0, 1500);

        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }

    EXPECT_TRUE(averageCallbackDelay[1] >= timer1ValidCountMin && averageCallbackDelay[1] <= timer1ValidCountMax);
    EXPECT_TRUE(averageCallbackDelay[2] >= timer2ValidCountMin && averageCallbackDelay[2] <= timer2ValidCountMax);
}