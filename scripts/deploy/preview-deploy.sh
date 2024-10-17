#!/bin/bash
set -e

tag="$1"
pr="$2"
branch="$3"

image="obroomhall/mediocre:$tag"
name="mediocre-preview-$pr"
port=$(printf "1%04d" $pr)

sanitised_branch=$(echo $branch | sed 's/\//-/')
vercel_domain="https://mediocre-configure-git-$sanitised_branch-mediocrity.vercel.app/"

./deploy.sh $image $name $port $vercel_domain
