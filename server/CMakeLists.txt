cmake_minimum_required(VERSION 3.10)

set(BINARY r-type_server)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(${BINARY} ${src})

if (WIN32)
    target_compile_options(${BINARY} PRIVATE /W4)
else()
    target_compile_options(${BINARY} PRIVATE -Wall -Wextra)
endif()

target_include_directories(
    ${BINARY} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${freaky-engine_SOURCE_DIR}/include
)

target_link_libraries(${BINARY} PUBLIC Boost::asio)
