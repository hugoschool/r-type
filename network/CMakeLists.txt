cmake_minimum_required(VERSION 3.10)

project(freaky-network LANGUAGES CXX VERSION ${r-type_VERSION})

file(GLOB_RECURSE src "src/*.cpp")
add_library(freaky-network ${src})

target_link_libraries(freaky-network PUBLIC Boost::asio)
target_include_directories(freaky-network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(freaky-network ${CMAKE_CURRENT_SOURCE_DIR}/include)
