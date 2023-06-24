ARG BASE_REPO=mediocre
ARG BASE_TAG=base-dev-local
FROM $BASE_REPO:$BASE_TAG

COPY . /src
RUN mkdir build
WORKDIR /src/build

RUN cmake ..
RUN cmake --build .

ENTRYPOINT ["./mediocre"]
EXPOSE 50051
