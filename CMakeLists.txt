cmake_minimum_required(VERSION 3.16)

project(CVRP)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.8.2
)
FetchContent_MakeAvailable(spdlog)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} spdlog)