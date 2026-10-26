cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(version 1.0.0)
project(freaky-td LANGUAGES CXX VERSION ${version})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

include(cmake/CPM.cmake)

# Currently pinned to a commit
set(FENGINE_VERSION 281db85da4d8c00ca5c76f03979c85bd6ade233b)
CPMAddPackage(
    NAME freaky-engine
    URL https://github.com/hugoschool/r-type/archive/${FENGINE_VERSION}.tar.gz
    OPTIONS "ONLY_BUILD_ENGINE ON"
)

set(BINARY freakytd)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(${BINARY} ${src})

if (MSVC)
    target_compile_options(${BINARY} PRIVATE /W4)
else()
    target_compile_options(${BINARY} PRIVATE -Wall -Wextra)
endif()

target_include_directories(
    ${BINARY} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${BINARY} PUBLIC freaky-engine)
