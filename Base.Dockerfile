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
ARG CMAKE_WORKDIR=/local/cmake
WORKDIR $CMAKE_WORKDIR
RUN --mount=type=cache,target=./download \
    mkdir ./install \
    && wget -q -O ./download/cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION\-linux-x86_64.sh \
    && sh ./download/cmake-linux.sh -- --skip-license --prefix=./install
ENV PATH=$PATH:$CMAKE_WORKDIR/install/bin

# install opencv
ARG OPENCV_VERSION=4.7.0
ARG OPENCV_WORKDIR=/local/opencv
WORKDIR $OPENCV_WORKDIR
RUN --mount=type=cache,target=./download \
    --mount=type=cache,target=./build \
    mkdir ./install \
    && wget -q -O ./download/source.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip \
    && ls ./download \
    && unzip -o ./download/source.zip -d ./download \
    && cmake -D CMAKE_INSTALL_PREFIX=./install \
             -D BUILD_opencv_highgui=OFF \
             -D BUILD_PROTOBUF=OFF \
             -D BUILD_TESTS=OFF \
             -D BUILD_PERF_TESTS=OFF \
             -S ./download/opencv-$OPENCV_VERSION \
             -B ./build \
    && cmake --build ./build \
    && make -j 4 -C ./build \
    && make install -C ./build
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$OPENCV_WORKDIR/install/lib/cmake

# install grpc and protobuf
ARG GRPC_VERSION=1.55.1
ARG GRPC_WORKDIR=/local/grpc
WORKDIR $GRPC_WORKDIR
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y autoconf libtool pkg-config
RUN --mount=type=cache,target=./download \
    --mount=type=cache,target=./build \
    mkdir ./install \
    && git clone --recurse-submodules -b v$GRPC_VERSION --depth 1 --shallow-submodules https://github.com/grpc/grpc ./download/grpc \
    && cmake -D gRPC_INSTALL=ON \
             -D gRPC_BUILD_TESTS=OFF \
             -D CMAKE_INSTALL_PREFIX=./install \
             -S ./download/grpc \
             -B ./build \
    && make -j 4 -C ./build \
    && make install -C ./build
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$GRPC_WORKDIR/install/lib/cmake

# install grpc client cli
ARG GRPC_CLIENT_CLI_VERSION=1.18.0
ARG GRPC_CLIENT_CLI_WORKDIR=/local/cmake
WORKDIR $GRPC_CLIENT_CLI_WORKDIR
WORKDIR /local/grpc-client-cli
RUN --mount=type=cache,target=./download \
    mkdir ./install \
    && wget -q -O ./download/source.tar.gz https://github.com/vadimi/grpc-client-cli/releases/download/v$GRPC_CLIENT_CLI_VERSION/grpc-client-cli_linux_x86_64.tar.gz \
    && tar -xz ./download/source.tar.gz -C ./install
ENV PATH=$PATH:$GRPC_CLIENT_CLI_WORKDIR/install/bin

# install leptonica
ARG LEPTONICA_VERSION=1.83.1
ARG LEPTONICA_WORKDIR=/local/leptonica
WORKDIR $LEPTONICA_WORKDIR
RUN --mount=type=cache,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y libicu-dev libpango1.0-dev libcairo2-dev libtiff-dev libjpeg-dev
RUN --mount=type=cache,target=./download \
    --mount=type=cache,target=./build \
    mkdir ./install \
    && wget -q -O ./download/source.zip https://github.com/DanBloomberg/leptonica/archive/refs/tags/$LEPTONICA_VERSION.zip \
    && unzip -o ./download/source.zip -d ./download \
    && cmake -D CMAKE_INSTALL_PREFIX=./install \
             -S ./download/leptonica-$LEPTONICA_VERSION \
             -B ./build \
    && make -j 4 -C ./build \
    && make install -C ./build
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$LEPTONICA_WORKDIR/install/lib/cmake

# install tesseract
ARG TESSERACT_VERSION=5.2.0
ARG TESSERACT_WORKDIR=/local/tesseract
WORKDIR $TESSERACT_WORKDIR
RUN --mount=type=cache,target=./download \
    --mount=type=cache,target=./build \
    mkdir ./install \
    && wget -q -O source.zip https://github.com/tesseract-ocr/tesseract/archive/refs/tags/$TESSERACT_VERSION.zip \
    && unzip -o ./download/source.zip -d ./download \
    && cmake -D CMAKE_INSTALL_PREFIX=./install \
             -S ./download/tesseract-$TESSERACT_VERSION \
             -B ./build \
    && make -j 4 -C ./build \
    && make install -C ./build
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$TESSERACT_WORKDIR/install/lib/cmake

# install tesseract languages
ARG TESSDATA_VERSION=4.1.0
ENV TESSDATA_PREFIX=/local/tesseract/install/share/tessdata/
RUN wget -q -P $TESSDATA_PREFIX https://github.com/tesseract-ocr/tessdata_best/raw/$TESSDATA_VERSION/eng.traineddata
