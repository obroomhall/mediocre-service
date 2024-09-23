#!/bin/bash
# Builds the library using registry cache

github_repo='obroomhall/mediocre-service'
dockerhub_repo='obroomhall/mediocre'
branch=$(git branch --show-current)

cache_tags=()
cache_tag_prefix="--cache-from type=registry,ref=${dockerhub_repo}"

if [[ $branch != 'master' ]]; then
  # commit sha cache
  sha=$(git rev-parse --short HEAD)
  if [[ $sha ]]; then
    cache_tags+=("${cache_tag_prefix}:sha-${sha}-develop-cache")
  fi

  # pr cache
  pr=$(curl -s https://api.github.com/repos/${github_repo}/pulls | jq ".[] | select(.head.ref == \"$branch\") | .number" | head -1)
  if [[ $pr ]]; then
    cache_tags+=("${cache_tag_prefix}:pr-${pr}-develop-cache")
  fi
fi

cache_tags+=("${cache_tag_prefix}:master-develop-cache")

docker buildx build \
  --force-rm \
  --progress plain \
  ${cache_tags[@]} \
  --target develop \
  -t mediocre:local-develop .