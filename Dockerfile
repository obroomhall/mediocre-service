FROM ubuntu:22.04 AS builder

# use bash instead of sh
SHELL ["/bin/bash", "-c"]

# installation tools
RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y wget git unzip

# install build tools and debugger
RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y build-essential gdb

# install cmake
ARG CMAKE_VERSION=3.25.3
RUN mkdir -p /local/cmake && cd "$_" \
    && mkdir ./download && mkdir ./install \
    && wget -q -O ./download/cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION\-linux-x86_64.sh \
    && sh ./download/cmake-linux.sh -- --skip-license --prefix=./install
ENV PATH=$PATH:/local/cmake/install/bin

# install ninja
ARG NINJA_VERSION=1.11.1
RUN mkdir -p /local/ninja && cd "$_" \
    && mkdir ./download && mkdir ./install \
    && wget -q -O ./download/source.zip https://github.com/ninja-build/ninja/releases/download/v$NINJA_VERSION/ninja-linux.zip \
    && unzip ./download/source.zip -d ./install
ENV PATH=$PATH:/local/ninja/install

# install grpc and protobuf
ARG GRPC_VERSION=1.55.1
RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
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
    && cmake -D gRPC_BUILD_TESTS=OFF \
             -D CMAKE_INSTALL_PREFIX=../install \
             -G Ninja \
             -S ../download/grpc \
             -B . \
    && ninja install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/grpc/install/lib/cmake

# install opencv
# we should be building staticly, but see https://github.com/opencv/opencv/issues/21447#issuecomment-1013088996
ARG OPENCV_VERSION=4.7.0
RUN --mount=type=cache,target=/local/opencv/build \
    cd /local/opencv/build \
    && mkdir ../download && mkdir ../install \
    && wget -q -O ../download/source.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip \
    && unzip ../download/source.zip -d ../download \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -D BUILD_opencv_highgui=OFF \
             -D WITH_PROTOBUF=ON \
             -D BUILD_PROTOBUF=OFF \
             -D BUILD_TESTS=OFF \
             -D BUILD_PERF_TESTS=OFF \
             -G Ninja \
             -S ../download/opencv-$OPENCV_VERSION \
             -B . \
    && ninja install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/opencv/install/lib/cmake

# install leptonica
ARG LEPTONICA_VERSION=1.83.1
RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y libicu-dev libpango1.0-dev libcairo2-dev libtiff-dev libjpeg-dev
RUN --mount=type=cache,target=/local/leptonica/build \
    cd /local/leptonica/build \
    && mkdir ../download && mkdir ../install \
    && wget -q -O ../download/source.zip https://github.com/DanBloomberg/leptonica/archive/refs/tags/$LEPTONICA_VERSION.zip \
    && unzip ../download/source.zip -d ../download \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -G Ninja \
             -S ../download/leptonica-$LEPTONICA_VERSION \
             -B . \
    && ninja install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/leptonica/install/lib/cmake

# install tesseract
ARG TESSERACT_VERSION=5.2.0
RUN --mount=type=cache,target=/local/tesseract/build \
    cd /local/tesseract/build \
    && mkdir ../download && mkdir ../install \
    && wget -q -O ../download/source.zip https://github.com/tesseract-ocr/tesseract/archive/refs/tags/$TESSERACT_VERSION.zip \
    && unzip ../download/source.zip -d ../download \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -G Ninja \
             -S ../download/tesseract-$TESSERACT_VERSION \
             -B . \
    && ninja install
ENV CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:/local/tesseract/install/lib/cmake

# install tesseract languages
ARG TESSDATA_VERSION=4.1.0
RUN wget -q -P /local/tesseract/install/share/tessdata/ https://github.com/tesseract-ocr/tessdata_best/raw/$TESSDATA_VERSION/eng.traineddata

# install grpc client cli
ARG GRPC_CLIENT_CLI_VERSION=1.18.0
RUN mkdir -p /local/grpc-client-cli && cd "$_" \
    && mkdir ./download && mkdir ./install \
    && wget -q -O ./download/source.tar.gz https://github.com/vadimi/grpc-client-cli/releases/download/v$GRPC_CLIENT_CLI_VERSION/grpc-client-cli_linux_x86_64.tar.gz \
    && tar -xzf ./download/source.tar.gz -C ./install
ENV PATH=$PATH:/local/grpc-client-cli/install

# install mediocre
RUN --mount=type=bind,source=mediocre,target=/local/mediocre/source/mediocre \
    --mount=type=bind,source=grpc,target=/local/mediocre/source/grpc \
    --mount=type=bind,source=CMakeLists.txt,target=/local/mediocre/source/CMakeLists.txt \
    --mount=type=cache,target=/local/mediocre/build \
    cd /local/mediocre/build \
    && mkdir ../install \
    && cmake -D CMAKE_INSTALL_PREFIX=../install \
             -G Ninja \
             -S ../source \
             -B . \
    && ninja install
ENV PATH=$PATH:/local/mediocre/install/bin


FROM ubuntu:22.04

# install required shared libraries
# opencv also builds these, can we use those instead?
RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y libicu-dev libpango1.0-dev libcairo2-dev libtiff-dev libjpeg-dev

# copy required files/settings from builder
ENV TESSDATA_PREFIX=/local/tesseract/install/share/tessdata/
COPY --from=builder $TESSDATA_PREFIX $TESSDATA_PREFIX
COPY --from=builder /local/grpc-client-cli/install /app/bin
# see "why can't you just be normal?" meme for why this is necessary
COPY --from=builder /local/opencv/install/lib/libopencv_imgproc.so.* /local/opencv/install/lib/
COPY --from=builder /local/mediocre/install /app
ENV PATH=$PATH:/app/bin

# run mediocre
ENTRYPOINT ["mediocre"]
EXPOSE 50051
HEALTHCHECK --interval=30s --timeout=3s \
  CMD grpc-client-cli health 127.0.0.1:50051