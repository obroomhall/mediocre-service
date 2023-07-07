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
            --cache-from type=registry,ref=${repo}:sha-${sha}-cache \
            --cache-from type=registry,ref=${repo}:pr-${pr}-cache \
            --cache-from type=registry,ref=${repo}:master-cache \
            --target builder \
            -t mediocre:local-builder .
else
    docker buildx build \
        --force-rm \
        --progress plain \
        --cache-from type=registry,ref=${repo}:master-cache \
        --target builder \
        -t mediocre:local-builder .
fi
