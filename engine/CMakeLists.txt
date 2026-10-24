cmake_minimum_required(VERSION 3.10)

set(version 1.0.0)
project(freaky-engine LANGUAGES CXX VERSION ${version})

file(GLOB_RECURSE src "src/*.cpp")
add_library(freaky-engine ${src})

target_link_libraries(freaky-engine PUBLIC sfml-graphics sfml-window)

include_directories(freaky-engine ${CMAKE_CURRENT_SOURCE_DIR}/include)
