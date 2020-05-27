# Raytracer


This repository contains a raytracer for University of Helsinki's course Data Structures Project.
The course has ended but development of this project continues. 

[![Build Status](https://travis-ci.org/Tykky/Raytracer.svg?branch=master)](https://travis-ci.org/Tykky/Raytracer)
[![codecov](https://codecov.io/gh/Tykky/Raytracer/branch/master/graph/badge.svg)](https://codecov.io/gh/Tykky/Raytracer)
## Documentation

[Project Specification](./Documentation/specification.md)

[Project Implementation](./Documentation/implementation.md)

[Project Testing](./Documentation/testing.md)

[Doxygen doc](https://tykky.github.io/)

## Dependencies

* [googletest](https://github.com/google/googletest) (via git submodule, use --recursive while cloning the repository)
* [cmake 3.12.4 (or newer)](https://cmake.org/)
* [OpenMP](https://www.openmp.org/)

## How to build

````
git clone https://github.com/Tykky/Raytracer --recursive
cd raytracer
mkdir build
cd build
cmake ..
cmake --build . --config release
````
## Current state of the project

![](Documentation/data/renders/demo.png)
The renderer is now more efficient in rendering 
multiple objects. This is due to use of bounding 
volume hierarchy data structure. Hit testing is done 
in logarithmic time O(log(n)) instead of linear.
