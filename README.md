# Raytracer

A Monte Carlo Path Tracer built from scratch.

![linux](https://github.com/Tykky/Raytracer/workflows/Linux/badge.svg)
[![codecov](https://codecov.io/gh/Tykky/Raytracer/branch/master/graph/badge.svg)](https://codecov.io/gh/Tykky/Raytracer)

## Dependencies
* [googletest](https://github.com/google/googletest) (via git submodule)
* [glfw](https://github.com/glfw/glfw) (via git submodule)
* [glew](https://github.com/nigels-com/glew) (via git submodule)
* OpenMP
* OpenGL

## How to build
````
git clone https://github.com/Tykky/Raytracer --recursive
cd Raytracer
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config release
cd bin
````
## Run after building
```
./raytracer
```

## Current state of the project
![](doc/data/gui2.jpg)

3D-models in .obj file format are now supported.

## Gallery

![](doc/data/renders/stanford_dragon.jpg)
![](doc/data/renders/demo.png)
![](doc/data/renders/demo2.png)
![](doc/data/renders/image3.png)
