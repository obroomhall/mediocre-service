#!/bin/bash
set -e

image="obroomhall/mediocre:master"
name="mediocre-production"
port=443
allowed_origins=https://configure.mediocre.tv

echo -------
echo image: $image
echo port: $port
echo allowed_origins: $allowed_origins

echo
echo -------
echo Pulling $image
docker pull $image

if docker container inspect $name > /dev/null 2>&1; then
    echo
    echo -------
    echo Destroying $name
    docker stop $name
    docker rm $name
fi

echo
echo -------
echo Running $name
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