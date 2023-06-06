FROM ubuntu:22.04

# install c++ build tools and debugger
RUN apt-get update && apt-get install -y build-essential gdb

# install cmake 3.25 (higher not yet supported by protobuf)
RUN apt-get update && apt-get install -y wget
RUN mkdir /opt/cmake && cd /opt/cmake \
     && wget -q https://github.com/Kitware/CMake/releases/download/v3.25.3/cmake-3.25.3-linux-x86_64.tar.gz \
     && tar xzf cmake-3.25.3-linux-x86_64.tar.gz \
     && ln -s /opt/cmake/cmake-3.25.3-linux-x86_64/bin/* /usr/local/bin/ \
     && cmake --version

# install protobuf compiler and libraries
RUN apt-get update && apt-get install -y protobuf-compiler libprotobuf-dev
