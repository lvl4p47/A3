#!/bin/bash

REQUIRED_PKG="libcunit1-dev"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
echo Checking for $REQUIRED_PKG: $PKG_OK
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Please install $REQUIRED_PKG."
  echo "Abort."
  exit
fi

if [ ! -d "build" ]; then
    echo "Creating build directory ..."
    mkdir build
fi

cd build

if [ ! -d "tests" ]; then
    echo "Creating tests subdirectory ..."
    mkdir tests
fi

cd tests

# build tests
echo "Build tests ..."
gcc -o mod_test.tests ../../tests/mod_test.c ../../src/func.c -lcunit

# run tests
echo "Run tests ..."
./mod_test.tests
