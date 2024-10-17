#!/bin/bash
set -e

image="obroomhall/mediocre:master"
name="mediocre-production"
port=443
allowed_origins=https://configure.mediocre.tv

./deploy.sh $image $name $port $allowed_origins