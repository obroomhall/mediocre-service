FROM ubuntu:22.04

# higher than cmake 3.25.x is not yet supported by protobuf
ARG CMAKE_VERSION=3.25.3
ARG CMAKE_BUILD="cmake-$CMAKE_VERSION-linux-x86_64"

# install c++ build tools and debugger
RUN apt-get update && apt-get install -y build-essential gdb

# install cmake
RUN apt-get update && apt-get install -y wget
RUN mkdir /opt/cmake && cd /opt/cmake \
     && wget -q https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/$CMAKE_BUILD.tar.gz \
     && tar xzf $CMAKE_BUILD.tar.gz \
     && ln -s /opt/cmake/$CMAKE_BUILD/bin/* /usr/local/bin/ \
     && cmake --version

# install protobuf compiler and libraries
RUN apt-get update && apt-get install -y protobuf-compiler libprotobuf-dev
