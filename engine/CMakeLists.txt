cmake_minimum_required(VERSION 3.10)

# Project is there to be used as a SOURCE_DIR anchor
project(freaky-engine LANGUAGES CXX VERSION ${r-type_VERSION})

include_directories(
    freaky-engine INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
