FROM ubuntu:22.04

# use bash instead of sh
SHELL ["/bin/bash", "-c"]

# installation tools
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y wget git unzip

# install build tools and debugger
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y build-essential gdb

# install cmake
ARG CMAKE_VERSION=3.25.3
RUN mkdir -p /local/cmake && cd "$_" \
    && mkdir ./download && mkdir ./install \
    && wget -q -O ./download/cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION\-linux-x86_64.sh \
    && sh ./download/cmake-linux.sh -- --skip-license --prefix=./install
ENV PATH=$PATH:/local/cmake/install/bin

# install opencv
ARG OPENCV_VERSION=4.7.0
RUN --mount=type=cache,target=/local/opencv/build \
    cd /local/opencv/build \
    && mkdir ../download && mkdir ../install \
    && wget -q -O ../download/source.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip \
    && unzip ../download/source.zip -d ../download \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -D BUILD_opencv_highgui=OFF \
             -D BUILD_PROTOBUF=OFF \
             -D BUILD_TESTS=OFF \
             -D BUILD_PERF_TESTS=OFF \
             -S ../download/opencv-$OPENCV_VERSION \
             -B . \
    && cmake --build . \
    && make -j 4 \
    && make install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/opencv/install/lib/cmake

# install grpc and protobuf
ARG GRPC_VERSION=1.55.1
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y autoconf libtool pkg-config
RUN --mount=type=cache,target=/local/grpc/download \
    --mount=type=cache,target=/local/grpc/build \
    cd /local/grpc/build \
    && mkdir ../install \
    && ([ -d ../download/grpc ] \
        && (pushd ../download/grpc \
          && ([[ $(git describe --tags) != v$GRPC_VERSION ]] \
            && (git fetch origin v$GRPC_VERSION:v$GRPC_VERSION --recurse-submodules --depth 1 \
                && git switch --detach v$GRPC_VERSION \
                && git submodule update --single-branch) \
            || echo "Tags are equal") \
          && popd) \
        || (git clone --recurse-submodules -b v$GRPC_VERSION --depth 1 --shallow-submodules https://github.com/grpc/grpc ../download/grpc)) \
    && cmake -D BUILD_SHARED_LIBS=ON \
             -D gRPC_BUILD_TESTS=OFF \
             -D CMAKE_INSTALL_PREFIX=../install \
             -S ../download/grpc \
             -B . \
    && cmake --build . \
    && make -j 4 \
    && make install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/grpc/install/lib/cmake

# install grpc client cli
ARG GRPC_CLIENT_CLI_VERSION=1.18.0
RUN mkdir -p /local/grpc-client-cli && cd "$_" \
    && mkdir ./download && mkdir ./install \
    && wget -q -O ./download/source.tar.gz https://github.com/vadimi/grpc-client-cli/releases/download/v$GRPC_CLIENT_CLI_VERSION/grpc-client-cli_linux_x86_64.tar.gz \
    && tar -xzf ./download/source.tar.gz -C ./install
ENV PATH=$PATH:/local/grpc-client-cli/install/bin

# install leptonica
ARG LEPTONICA_VERSION=1.83.1
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y libicu-dev libpango1.0-dev libcairo2-dev libtiff-dev libjpeg-dev
RUN --mount=type=cache,target=/local/leptonica/build \
    cd /local/leptonica/build \
    && mkdir ../download && mkdir ../install \
    && wget -q -O ../download/source.zip https://github.com/DanBloomberg/leptonica/archive/refs/tags/$LEPTONICA_VERSION.zip \
    && unzip ../download/source.zip -d ../download \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -S ../download/leptonica-$LEPTONICA_VERSION \
             -B . \
    && make -j 4 \
    && make install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/leptonica/install/lib/cmake

# install tesseract
ARG TESSERACT_VERSION=5.2.0
RUN --mount=type=cache,target=/local/tesseract/build \
    cd /local/tesseract/build \
    && mkdir ../download && mkdir ../install \
    && wget -q -O ../download/source.zip https://github.com/tesseract-ocr/tesseract/archive/refs/tags/$TESSERACT_VERSION.zip \
    && unzip ../download/source.zip -d ../download \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -S ../download/tesseract-$TESSERACT_VERSION \
             -B . \
    && make -j 4 \
    && make install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$TESSERACT_WORKDIR/install/lib/cmake

# install tesseract languages
ARG TESSDATA_VERSION=4.1.0
ENV TESSDATA_PREFIX=/local/tesseract/install/share/tessdata/
RUN wget -q -P $TESSDATA_PREFIX https://github.com/tesseract-ocr/tessdata_best/raw/$TESSDATA_VERSION/eng.traineddata
