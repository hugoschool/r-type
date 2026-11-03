cmake_minimum_required(VERSION 3.10)

set(BINARY r-type_client)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(${BINARY} ${src})

if (MSVC)
    target_compile_options(${BINARY} PRIVATE /W4)
else()
    target_compile_options(${BINARY} PRIVATE -Wall -Wextra -g3)
endif()

target_include_directories(
    ${BINARY} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${BINARY} PUBLIC
    freaky-engine-ecs
    freaky-engine-module-display
    freaky-engine-module-sound
    freaky-engine-observer
    freaky-engine-asset-manager
    freaky-network
    r-type_common
    r-type_proto
    Boost::asio
    Boost::serialization
)

# Force the compiler to use the static library
target_link_directories(${BINARY} PUBLIC ${protobuf_BINARY_DIR})
target_link_libraries(
    ${BINARY} PUBLIC
    protobuf::libprotobuf
)
