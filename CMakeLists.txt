cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(version 1.0.0)
project(r-type LANGUAGES CXX VERSION ${version})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

include(cmake/CPM.cmake)

set(BOOST_VERSION 1.92.0)
CPMAddPackage(
    NAME Boost
    URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-cmake.tar.xz
)

add_subdirectory(engine)
add_subdirectory(client)
add_subdirectory(server)
