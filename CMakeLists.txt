cmake_minimum_required(VERSION 3.7)
project(async)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES library.cpp library.h pool/ThreadPool.cpp pool/ThreadPool.h pool/Worker.cpp pool/Worker.h)
add_library(async ${SOURCE_FILES})
add_executable(async_app main.cpp)
add_dependencies(async_app async)
target_link_libraries(async_app async)