#!/bin/sh

echo "Install script"
set -e
engine_dir="$1"

mkdir -p "$engine_dir/build-android/include/pEp"
cp "$2"/src/*.h* "$engine_dir/build-android/include/pEp"
