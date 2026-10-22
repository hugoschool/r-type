cmake_minimum_required(VERSION 3.10)

project(r-type_common LANGUAGES CXX VERSION ${r-type_VERSION})

file(GLOB_RECURSE src "src/*.cpp")
add_library(r-type_common ${src})

include_directories(
    ${PROJECT_NAME}
    ${freaky-engine_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
