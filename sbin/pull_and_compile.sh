#!/bin/bash
git pull --recurse-submodules
mkdir build
cd build
cmake ..
make -j$1
