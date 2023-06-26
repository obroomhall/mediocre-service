ARG BASE_REPO=mediocre
ARG BASE_TAG=base-dev-local
FROM $BASE_REPO:$BASE_TAG

COPY libmediocre/ /src/libmediocre/
COPY CMakeLists.txt /src/
WORKDIR /src/build/

RUN cmake ..
RUN cmake --build .

ENTRYPOINT ["./mediocre"]
EXPOSE 50051
HEALTHCHECK --interval=30s --timeout=3s \
  CMD grpc-client-cli health 127.0.0.1:50051
