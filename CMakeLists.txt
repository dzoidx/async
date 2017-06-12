cmake_minimum_required(VERSION 3.7)
set(PROJECT_NAME_STR async)
project(${PROJECT_NAME_STR})

set(CMAKE_CXX_STANDARD 11)

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/pool ${PROJECT_SOURCE_DIR}/timer)
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

set(SOURCE_FILES pool/ThreadPool.cpp pool/ThreadPool.h pool/Worker.cpp pool/Worker.h timer/TimerDesc.cpp timer/TimerDesc.h pool/CachedThreadPool.cpp pool/CachedThreadPool.h timer/TimerQueue.cpp timer/TimerQueue.h timer/Timer.cpp timer/Timer.h)
add_library(async ${SOURCE_FILES} Definitions.h)
add_executable(async_app main.cpp)
add_dependencies(async_app async)
target_link_libraries(async_app async)

enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

target_link_libraries(${PROJECT_TEST_NAME} async)

if(NOT WIN32 OR MINGW)
    target_link_libraries(${PROJECT_TEST_NAME}
            ${GTEST_LIBS_DIR}/libgtest.a
            ${GTEST_LIBS_DIR}/libgtest_main.a
            )
else()
    target_link_libraries(${PROJECT_TEST_NAME}
            debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
            )
    target_link_libraries(${PROJECT_TEST_NAME}
            debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
            )
endif()

add_test(test1 ${PROJECT_TEST_NAME})