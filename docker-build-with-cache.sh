#!/bin/bash
# Builds the library using registry cache

repo='obroomhall/mediocre'
branch=$(git branch --show-current)
sha=$(git rev-parse --short HEAD)

if [[ $branch != 'master' ]]; then
    pr=$(curl -s https://api.github.com/repos/${repo}/pulls | jq ".[] | select(.head.ref == \"$branch\") | .number" | head -1)
    docker buildx build \
            --force-rm \
            --progress plain \
            --cache-from type=registry,ref=${repo}:sha-${sha}-develop-cache \
            --cache-from type=registry,ref=${repo}:pr-${pr}-develop-cache \
            --cache-from type=registry,ref=${repo}:master-develop-cache \
            --target develop \
            -t mediocre:local-develop .
else
    docker buildx build \
        --force-rm \
        --progress plain \
        --cache-from type=registry,ref=${repo}:master-develop-cache \
        --target develop \
        -t mediocre:local-develop .
fi
