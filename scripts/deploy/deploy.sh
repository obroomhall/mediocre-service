#!/bin/bash
set -e

image="$1"
name="$2"
port="$3"
allowed_origins="$4"

echo pull
docker pull $image

if docker container inspect $name > /dev/null 2>&1; then
    echo destroy
    ./destroy.sh $name
fi

echo run
docker run \
    -d \
    --name $name \
    --restart unless-stopped \
    -p $port:8443 \
    -v ./certs:/certs:ro \
    -e TLS_CERT_PATH=/certs/client.mediocre.tv.pem \
    -e TLS_KEY_PATH=/certs/client.mediocre.tv.key \
    -e ALLOWED_ORIGINS=$allowed_origins \
    $image