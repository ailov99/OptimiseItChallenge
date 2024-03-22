#!/bin/bash

case "$1" in
    "")
        echo "No args passed"
        exit 1;;
    "-b")
        cmake --build build
        echo "App built..."
        exit 1;;
    "-c")
        cmake -S . -B build
        echo "Cmake configured..."
        exit 1;;
    "-r")
        mkdir -p ./build/src && cp -r ./src/lua_drivers/* ./build/src/
        pushd .
        cd ./build/src
        ./main run
        popd
        exit 1;;
    "-t")
        mkdir -p ./build/src && cp -r ./src/lua_drivers/* ./build/src/
        pushd .
        cd ./build/src
        ./main test
        popd
        exit 1;;
esac
