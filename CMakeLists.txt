cmake_minimum_required(VERSION 3.10)
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

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 7.1.3
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.10.0
)
FetchContent_MakeAvailable(gtest)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
    set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
    set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()
FetchContent_MakeAvailable(glad)

FetchContent_Declare(
        linmath
        GIT_REPOSITORY https://github.com/datenwolf/linmath.h.git
)
FetchContent_GetProperties(linmath)
if(NOT linmath_POPULATED)
    FetchContent_Populate(linmath)
    add_library(linmath INTERFACE)
    target_include_directories(linmath INTERFACE ${linmath_SOURCE_DIR})
endif()
FetchContent_MakeAvailable(linmath)

include_directories(./include)

add_subdirectory(src/AntSystem)
add_subdirectory(src/CVRP)
add_subdirectory(src/Test)
