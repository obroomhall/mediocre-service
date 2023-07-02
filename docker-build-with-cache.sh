#!/bin/bash
# Builds the library using registry cache

repo='obroomhall/mediocre'
branch=$(git branch --show-current)

if [[ $branch != 'master' ]]; then
    pr=$(curl -s https://api.github.com/repos/${repo}/pulls | jq ".[] | select(.head.ref == \"$branch\") | .number" | head -1)
    docker buildx build \
            --force-rm \
            --progress plain \
            --cache-from type=registry,ref=${repo}:master-cache \
            --cache-from type=registry,ref=${repo}:${branch}-cache \
            --cache-from type=registry,ref=${repo}:pr-${pr}-cache \
            -t mediocre:local .
else
    docker buildx build \
        --force-rm \
        --progress plain \
        --cache-from type=registry,ref=${repo}:master-cache \
        -t mediocre:local .
fi
