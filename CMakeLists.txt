cmake_minimum_required(VERSION 3.7)
project(async)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES pool/ThreadPool.cpp pool/ThreadPool.h pool/Worker.cpp pool/Worker.h timer/TimerDesc.cpp timer/TimerDesc.h pool/CachedThreadPool.cpp pool/CachedThreadPool.h timer/TimerQueue.cpp timer/TimerQueue.h timer/Timer.cpp timer/Timer.h)
add_library(async ${SOURCE_FILES} Definitions.h)
add_executable(async_app main.cpp)
add_dependencies(async_app async)
target_link_libraries(async_app async)