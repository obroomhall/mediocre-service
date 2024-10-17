#!/bin/bash
set -e

name="$1"

docker stop $name
docker rm $name