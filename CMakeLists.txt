cmake_minimum_required(VERSION 3.25)
project(mediocre)

set(CMAKE_CXX_STANDARD 17)


## -- specify files to include -- ##

add_executable(
        mediocre
        libmediocre/main/main.cpp
        libmediocre/api/api.cpp
        libmediocre/test/test.cpp
)

add_library(
        proto-objects OBJECT
        libmediocre/main/main.proto
        libmediocre/api/api.proto
        libmediocre/test/test.proto
)


## -- integrate with protobuf/grpc -- ##

# find the protobuf cmake package
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

# find the protobuf cmake package
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

# set some reusable directory variables
set(PROTO_IMPORT_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(PROTO_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/Protobuf)
file(MAKE_DIRECTORY ${PROTO_BINARY_DIR})

# link up proto-objects
target_link_libraries(proto-objects PUBLIC protobuf::libprotobuf gRPC::grpc++)
target_include_directories(proto-objects PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

# generate protobuf files (.pb.h and .pb.cc)
protobuf_generate(
        TARGET proto-objects
        IMPORT_DIRS ${PROTO_IMPORT_DIRS}
        PROTOC_OUT_DIR ${PROTO_BINARY_DIR})

# generate grpc files (.grpc.pb.h and .grpc.pb.cc)
protobuf_generate(
        TARGET proto-objects
        LANGUAGE grpc
        GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
        PLUGIN "protoc-gen-grpc=\$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
        IMPORT_DIRS ${PROTO_IMPORT_DIRS}
        PROTOC_OUT_DIR ${PROTO_BINARY_DIR})


## -- link it all together -- ##

target_include_directories(mediocre PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(mediocre proto-objects)
