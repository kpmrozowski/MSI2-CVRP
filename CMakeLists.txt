cmake_minimum_required(VERSION 3.16)
project(CVRP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.8.2
)
FetchContent_MakeAvailable(spdlog)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} spdlog)