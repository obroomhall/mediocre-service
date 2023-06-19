FROM obroomhall/mediocre-base:latest

COPY . /src
RUN mkdir build
WORKDIR /src/build

RUN cmake ..
RUN cmake --build .

ENTRYPOINT ["/bin/bash"]
CMD ["mediocre"]
