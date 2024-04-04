#!/bin/bash


# This is a script to build the system using cmake (and building in the correct directory)

rm -rf build
mkdir build 
cd build 
cmake ..
cmake --build .

