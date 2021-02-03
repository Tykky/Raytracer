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

### **Note** 

Download [dragon.obj](http://tykky.github.io/dragon.obj) and place 
it in the same directory as the executable. Otherwise, the program won't run. 
This will be fixed soon so that the GUI has a button for importing 3D models.
Also note that you'll need to adjust camera position to (-0.2, 0.14, 0.0) to 
be able to see anything sensible. The original 3D model is from 
[The Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/).


## Gallery

![](doc/data/renders/stanford_dragon.jpg)
![](doc/data/renders/demo.png)
![](doc/data/renders/demo2.png)
![](doc/data/renders/image3.png)
