#!/bin/bash

case "$1" in
    "")
        echo "No args passed"
        exit 1;;
    "-b")
        cmake --build build
        cp -r ./src/lua_drivers/* ./build/src/
        echo "App built..."
        exit 1;;
    "-c")
        cmake -S . -B build
        echo "Cmake configured..."
        exit 1;;
    "-r")
        pushd .
        cd ./build/src
        ./main
        popd
        exit 1;;
esac
