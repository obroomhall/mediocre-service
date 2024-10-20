#!/bin/bash
set -e

image="$1"
pr="$2"
branch="$3"

name="mediocre-preview-$pr"
port=$(printf "1%04d" $pr)

sanitised_branch=$(echo $branch | sed 's/\//-/')
allowed_origins="https://mediocre-configure-git-$sanitised_branch-mediocrity.vercel.app/"

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
    -v ./certs/preview.mediocre.tv.pem:/certificates/cert.pem:ro \
    -v ./certs/preview.mediocre.tv.key:/certificates/key.pem:ro \
    -e ALLOWED_ORIGINS=$allowed_origins \
    $image