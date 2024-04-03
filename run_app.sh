#!/bin/bash

case "$1" in
    "")
        echo "No args passed"
        exit 1;;
    "-build")
        if test -d build; then
            cmake --build build
        else
            mkdir build
            cd build
            cmake -GNinja ..
            make
            cd ..
        fi
        echo "App built..."
        exit 1;;
    "-config")
        cmake -GNinja -S . -B build
        echo "Cmake configured..."
        exit 1;;
    "-clean")
        rm -r build
        echo "Build cleaned..."
        exit 1;;
    "-run")
        mkdir -p ./build/src && cp -r ./src/lua_drivers/* ./build/src/
        pushd .
        cd ./build/src
        ./main run
        popd
        exit 1;;
    "-test")
        mkdir -p ./build/src && cp -r ./src/lua_drivers/* ./build/src/
        mkdir -p ./build/src/test_data && cp -r ./src/lua_drivers/test_data/* ./build/src/test_data/
        pushd .
        cd ./build/src
        ./main test
        popd
        exit 1;;
esac
