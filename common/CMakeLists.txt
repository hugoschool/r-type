cmake_minimum_required(VERSION 3.10)

project(r-type_common LANGUAGES CXX VERSION ${r-type_VERSION})

file(GLOB_RECURSE src "src/*.cpp")
add_library(r-type_common ${src})

target_link_libraries(
    r-type_common PUBLIC
    freaky-engine-camera
    freaky-engine-config
    freaky-engine-ecs
    freaky-engine-scene
    Boost::serialization
)
target_include_directories(r-type_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include)
