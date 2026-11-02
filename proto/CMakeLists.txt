# https://github.com/protocolbuffers/protobuf/blob/2f9e8d5d20357457b61604cf077d65a6c6497792/docs/cmake_protobuf_generate.md
find_package(protobuf CONFIG REQUIRED)

# CPM workaround for protobuf
include("${protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake")

add_library(r-type_proto OBJECT "${CMAKE_CURRENT_LIST_DIR}/packets.proto")

target_link_libraries(r-type_proto PUBLIC protobuf)

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories(r-type_proto PUBLIC "${PROTO_BINARY_DIR}")

protobuf_generate(
    packets.proto
    TARGET r-type_proto
    IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}"
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
)
