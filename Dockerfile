ARG BASE_TAG=base-latest
FROM obroomhall/mediocre:$BASE_TAG

COPY . /src
RUN mkdir build
WORKDIR /src/build

RUN cmake ..
RUN cmake --build .

ENTRYPOINT ["/bin/bash"]
CMD ["mediocre"]
