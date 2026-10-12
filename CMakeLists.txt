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
set(SFML_VERSION 3.1.0)
CPMAddPackage(
    NAME SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/${SFML_VERSION}.tar.gz
    OPTIONS
        "SFML_BUILD_NETWORK OFF"
        "SFML_BUILD_DOC OFF"
        "SFML_BUILD_EXAMPLES OFF"
)

add_subdirectory(engine)
add_subdirectory(client)
add_subdirectory(server)
