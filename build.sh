#!/bin/bash

if [ ! -d "build" ]; then
    echo "Creating build directory ..."
    mkdir build
fi

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:./lib"

cd build
rm -rf *
cmake ..
make