# Raytracer


This repository contains a raytracer for University of Helsinki's course Data Structures Project.
The course has ended but development of this project continues. 

[![Build Status](https://travis-ci.org/Tykky/Raytracer.svg?branch=master)](https://travis-ci.org/Tykky/Raytracer)
[![codecov](https://codecov.io/gh/Tykky/Raytracer/branch/master/graph/badge.svg)](https://codecov.io/gh/Tykky/Raytracer)
## Documentation

[Documentation](https://tykky.github.io/)

## Dependencies

* [googletest](https://github.com/google/googletest) (via git submodule)
* [glfw](https://github.com/glfw/glfw) (via git submodule)
* [glew](https://github.com/nigels-com/glew) (via git submodule)
* [OpenMP](https://www.openmp.org/)
* [OpenGL](https://www.opengl.org/)

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

![](doc/data/renders/demo.png)
The renderer is now more efficient in rendering 
multiple objects. This is due to use of bounding 
volume hierarchy data structure. Hit testing is done 
in logarithmic time O(log(n)) instead of linear.
