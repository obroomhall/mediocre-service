FROM ubuntu:22.04

# check for CLion support before upgrading cmake
ARG CMAKE_VERSION=3.25.3
ARG GRPC_VERSION=1.55.1
ARG OPENCV_VERSION=4.7.0
ARG LEPTONICA_VERSION=1.83.1
ARG TESSERACT_VERSION=5.2.0
ARG TESSDATA_VERSION=4.1.0
ARG GRPC_CLIENT_CLI_VERSION=1.18.0

ARG CMAKE_INSTALL_PREFIX=$HOME/local
ARG CMAKE_INSTALL_PREFIX_CMAKE=$CMAKE_INSTALL_PREFIX/cmake
ARG CMAKE_INSTALL_PREFIX_GRPC=$CMAKE_INSTALL_PREFIX/grpc
ARG CMAKE_INSTALL_PREFIX_OPENCV=$CMAKE_INSTALL_PREFIX/opencv
ARG CMAKE_INSTALL_PREFIX_LEPTONICA=$CMAKE_INSTALL_PREFIX/leptonica
ARG CMAKE_INSTALL_PREFIX_TESSERACT=$CMAKE_INSTALL_PREFIX/tesseract

# use bash instead of sh
SHELL ["/bin/bash", "-c"]

# installation tools
RUN apt-get update  \
    && apt-get install -y wget git unzip

# install build tools and debugger
RUN apt-get update  \
    && apt-get install -y build-essential gdb


# install cmake

RUN wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION\-linux-x86_64.sh \
    && mkdir -p $CMAKE_INSTALL_PREFIX/cmake \
    && sh cmake-linux.sh -- --skip-license --prefix=$CMAKE_INSTALL_PREFIX_CMAKE \
    && rm cmake-linux.sh

ENV PATH=$PATH:$CMAKE_INSTALL_PREFIX_CMAKE/bin


# install opencv

RUN wget -q -O opencv.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip \
    && unzip opencv.zip \
    && mkdir -p opencv-$OPENCV_VERSION/build $CMAKE_INSTALL_PREFIX_OPENCV \
    && pushd opencv-$OPENCV_VERSION/build \
    && cmake -D CMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX_OPENCV \
             -D BUILD_opencv_highgui=OFF \
             -D BUILD_PROTOBUF=OFF \
             -D BUILD_TESTS=OFF \
             -D BUILD_PERF_TESTS=OFF \
             -D BUILD_SHARED_LIBS=OFF \
             .. \
    && cmake --build . \
    && make -j 4 \
    && make install \
    && popd \
    && rm -rf opencv-$OPENCV_VERSION opencv.zip

ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$CMAKE_INSTALL_PREFIX_OPENCV/lib/cmake


# install grpc, protobuf and tools

RUN apt-get update  \
    && apt-get install -y autoconf libtool pkg-config

RUN git clone --recurse-submodules -b v$GRPC_VERSION --depth 1 --shallow-submodules https://github.com/grpc/grpc \
    && mkdir -p grpc/cmake/build $CMAKE_INSTALL_PREFIX_GRPC \
    && pushd grpc/cmake/build \
    && cmake -DgRPC_INSTALL=ON \
             -DgRPC_BUILD_TESTS=OFF \
             -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX_GRPC \
             ../.. \
    && make -j 4 \
    && make install \
    && popd \
    && rm -rf grpc

ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$CMAKE_INSTALL_PREFIX_GRPC/lib/cmake

RUN wget -O - https://github.com/vadimi/grpc-client-cli/releases/download/v$GRPC_CLIENT_CLI_VERSION/grpc-client-cli_linux_x86_64.tar.gz | tar -C /usr/local/bin -xz


# install tesseract and leptonica

RUN apt-get update \
    && apt-get install -y libicu-dev libpango1.0-dev libcairo2-dev libtiff-dev libjpeg-dev

RUN wget -q -O leptonica.zip https://github.com/DanBloomberg/leptonica/archive/refs/tags/$LEPTONICA_VERSION.zip \
    && unzip leptonica.zip \
    && mkdir -p leptonica-$LEPTONICA_VERSION/build $CMAKE_INSTALL_PREFIX_LEPTONICA \
    && pushd leptonica-$LEPTONICA_VERSION/build \
    && cmake -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX_LEPTONICA .. \
    && make -j 4 \
    && make install \
    && popd \
    && rm -rf leptonica-$LEPTONICA_VERSION leptonica.zip

ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$CMAKE_INSTALL_PREFIX_LEPTONICA/lib/cmake

RUN wget -q -O tesseract.zip https://github.com/tesseract-ocr/tesseract/archive/refs/tags/$TESSERACT_VERSION.zip \
    && unzip tesseract.zip \
    && mkdir -p tesseract-$TESSERACT_VERSION/build $CMAKE_INSTALL_PREFIX_TESSERACT \
    && pushd tesseract-$TESSERACT_VERSION/build \
    && cmake -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX_TESSERACT .. \
    && make -j 4 \
    && make install \
    && popd \
    && rm -rf tesseract-$TESSERACT_VERSION tesseract.zip

ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:$CMAKE_INSTALL_PREFIX_TESSERACT/lib/cmake
ENV TESSDATA_PREFIX=$CMAKE_INSTALL_PREFIX_TESSERACT/share/tessdata/
RUN wget -q -P $TESSDATA_PREFIX https://github.com/tesseract-ocr/tessdata_best/raw/$TESSDATA_VERSION/eng.traineddata
