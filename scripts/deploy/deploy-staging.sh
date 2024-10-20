#!/bin/bash
set -e

image="obroomhall/mediocre:master"
name="mediocre-staging"
port=444

echo -------
echo image: $image
echo port: $port

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
    -v /etc/letsencrypt/live/staging.mediocre.tv/fullchain.pem:/certificates/cert.pem:ro \
    -v /etc/letsencrypt/live/staging.mediocre.tv/privkey.pem:/certificates/key.pem:ro \
    -e ALLOWED_ORIGINS=* \
    $image