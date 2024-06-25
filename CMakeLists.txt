# Setup project
cmake_minimum_required(VERSION 3.22.1)
project(3d-raytracer VERSION 1.0 DESCRIPTION "My Sample C++ Project")

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(3d-raytracer src/main.cpp)
