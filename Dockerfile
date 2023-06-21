ARG BASE_REPO=obroomhall/mediocre
ARG BASE_TAG=base-dev-master
FROM $BASE_REPO:$BASE_TAG

COPY . /src
RUN mkdir build
WORKDIR /src/build

RUN cmake ..
RUN cmake --build .

ENTRYPOINT ["./mediocre"]
