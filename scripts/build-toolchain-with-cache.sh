#!/bin/bash
# Builds the library using registry cache

repo='obroomhall/mediocre'
branch=$(git branch --show-current)

cache_tags=()

if [[ $branch != 'master' ]]; then
  # commit sha cache
  sha=$(git rev-parse --short HEAD)
  if [[ $sha ]]; then
    cache_tags+=(
      "--cache-from type=registry,ref=${repo}:sha-${sha}-develop-cache"
    )
  fi

  # pr cache
  pr=$(curl -s https://api.github.com/repos/${repo}/pulls | jq ".[] | select(.head.ref == \"$branch\") | .number" | head -1)
  if [[ $pr ]]; then
    cache_tags+=(
      "--cache-from type=registry,ref=${repo}:pr-${pr}-develop-cache"
    )
  fi
fi

cache_tags+=(
  "--cache-from type=registry,ref=${repo}:master-develop-cache"
)

docker buildx build \
  --force-rm \
  --progress plain \
  ${cache_tags[@]} \
  --target develop \
  -t mediocre:local-develop .