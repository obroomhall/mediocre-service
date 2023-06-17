FROM ubuntu:22.04

# check support in Clion > Build, Execution, Deployment > Toolchains before upgrading cmake
ARG CMAKE_VERSION="3.25.3"
ARG GRPC_VERSION="v1.55.1"

# use bash instead of sh (useful for pushd popd)
SHELL ["/bin/bash", "-c"]

# install required build tools
RUN apt-get update && apt-get install -y build-essential gdb autoconf libtool pkg-config wget git

# install cmake
RUN wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION\-linux-x86_64.sh \
    && sh cmake-linux.sh -- --skip-license --prefix=/usr/local \
    && rm cmake-linux.sh \
    && cmake --version

# install grpc and protobuf
RUN git clone --recurse-submodules -b $GRPC_VERSION --depth 1 --shallow-submodules https://github.com/grpc/grpc \
    && mkdir -p grpc/cmake/build \
    && pushd grpc/cmake/build \
    && cmake -DgRPC_INSTALL=ON \
             -DgRPC_BUILD_TESTS=OFF \
             ../.. \
    && make -j 4 \
    && make install \
    && popd \
    && rm -rf grpc
