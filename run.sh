#!/bin/bash

# Build the project
cmake -B build/Release -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release

cd build/Release/

./3d-raytracer > image.ppm

cd ../../


