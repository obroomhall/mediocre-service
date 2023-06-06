cmake_minimum_required(VERSION 3.25)
project(mediocre)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_LIST_DIR})
add_library(
        mediocre
        libmediocre/api/api.cpp
        libmediocre/test/test.cpp
)

## integrate with protobuf

# specify an output directory for proto files
set(MESSAGES_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/Generated)
file(MAKE_DIRECTORY ${MESSAGES_GENERATED_DIR})
include_directories(${MESSAGES_GENERATED_DIR})

# find the protobuf cmake package
find_package(Protobuf REQUIRED)

# generate all protobuf sources and headers
foreach (
        PROTO_FILE
        libmediocre/api/api.proto
        libmediocre/test/test.proto
)
    # handle parent output directory
    cmake_path(GET PROTO_FILE PARENT_PATH PROTO_DIR)
    set(GEN_DIR ${MESSAGES_GENERATED_DIR}/${PROTO_DIR})
    file(MAKE_DIRECTORY ${GEN_DIR})

    # generate source and header
    protobuf_generate_cpp(PROTO_SRC PROTO_HDR ${PROTO_FILE} PROTOC_OUT_DIR ${GEN_DIR})

    # append source and header to lists
    list(APPEND PROTO_SRCS ${PROTO_SRC})
    list(APPEND PROTO_HDRS ${PROTO_HDR})
endforeach ()

# link the libraries
add_library(proto ${PROTO_HDRS} ${PROTO_SRCS})
target_link_libraries(mediocre proto ${PROTOBUF_LIBRARY})
