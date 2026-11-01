cmake_minimum_required(VERSION 3.10)

set(version ${freaky-engine_VERSION})
set(project_name freaky-engine-asset-manager)
project(${project_name} LANGUAGES CXX VERSION ${version})

file(GLOB_RECURSE src "*.cpp")
add_library(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-audio)
target_include_directories(${PROJECT_NAME} PUBLIC ${freaky-engine_SOURCE_DIR}/include)
