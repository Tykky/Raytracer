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
cmake ..
cmake --build . --config release
cd bin
````
## Run after building
```
./raytracer
```

## Same in one-liner
```
git clone https://github.com/Tykky/Raytracer --recursive && cd Raytracer && mkdir build && cd build && cmake .. && cmake --build . --config release && cd bin && ./raytracer
```

## Current state of the project

![](doc/data/renders/demo.png)
The renderer is now more efficient in rendering 
multiple objects. This is due to use of bounding 
volume hierarchy data structure. Hit testing is done 
in logarithmic time O(log(n)) instead of linear.
