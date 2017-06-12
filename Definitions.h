//
// Created by morgan on 12.06.2017.
//

#ifndef ASYNC_DEFINITIONS_H
#define ASYNC_DEFINITIONS_H

#include <chrono>

namespace async {

    using Clock = std::chrono::high_resolution_clock;
    using SystemClock = std::chrono::system_clock;
}

#endif //ASYNC_DEFINITIONS_H
