#!/bin/bash
set -e

pr="$1"
name="mediocre-preview-$pr"

./destroy.sh $name