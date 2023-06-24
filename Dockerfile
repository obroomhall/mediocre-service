ARG BASE_REPO=obroomhall/mediocre
ARG BASE_TAG=base-dev-master
FROM $BASE_REPO:$BASE_TAG

COPY . /src
RUN mkdir build
WORKDIR /src/build

RUN cmake ..
RUN cmake --build .

ARG TESSDATA_VERSION=4.1.0
ENV TESSDATA_PREFIX=/usr/local/share/tessdata/
RUN wget -q -P $TESSDATA_PREFIX https://github.com/tesseract-ocr/tessdata_best/raw/$TESSDATA_VERSION/eng.traineddata

ENTRYPOINT ["./mediocre"]
