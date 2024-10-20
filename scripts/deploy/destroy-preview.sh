#!/bin/bash
set -e

pr="$1"
name="mediocre-preview-$pr"

echo -------
echo Destroying $name
docker stop $name
docker rm $name