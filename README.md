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

### To run

* [QT 5.14.2](https://www.qt.io/download-open-source)

### To build, test and develop
* [googletest](https://github.com/google/googletest) (via git submodule, use --recursive while cloning the repository)
* [cmake 3.12.4 (or newer)](https://cmake.org/)
* [GCC 9.2 (or newer)](https://gcc.gnu.org/)
* [git 2.26.2 (or newer)](https://git-scm.com/)
* [OpenMP (comes with GCC)](https://www.openmp.org/)

## How to build

Before building (or running) make sure you have the required 
dependencies installed. All of the dependencies are cross-platform
and the project can be built at least on Windows, Linux and macOS. 
**Note that googletest comes with the project when cloning is done with --recursive option.**

### On Linux
#### Building
```
git clone --recursive https://github.com/Tykky/Raytracer
cd Raytracer
mkdir build
cd build
cmake ..
make
```
#### Running
After building the program can be run with:
```
./Source/raytracer
```
Tests can be run with:
```
./Test/gtest_run
```

### On Windows

#### Installing dependencies
On windows the project can be built using MinGW.
MinGW can be installed with [msys2](https://www.msys2.org/) and more 
specifically the group [mingw-w64-x86_64-toolchain](https://packages.msys2.org/group/mingw-w64-x86_64-toolchain)
is the one you want to install. QT, cmake and git can be also acquired with msys2. When you have [msys2](https://www.msys2.org/) installed,
all of these packages can be installed with (make sure you add msys2 to path variable!):
```
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-qt5 mingw-w64-x86_64-cmake git
```
#### Building
When you have everything ready, the project can be built with:
```
git clone --recursive https://github.com/Tykky/Raytracer
cd .\Raytracer\
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```
#### Running
After this you should be able to run the program with
```
.\Source\raytracer.exe
```
and run the tests with 
```
.\Test\gtest_run.exe
```
## Graphical user interface
The gui is work in progress. 

## Current state of the project

![](Documentation/data/renders/demo.png)
The renderer is now more efficient in rendering 
multiple objects. This is due to use of bounding 
volume hierarchy data structure. Hit testing is done 
in logarithmic time O(log(n)) instead of linear.

## License

The software (Raytracer) is released under [MIT License](./LICENSE).
QT Library is used to provide cross-platform graphical 
user interface and is dynamically linked to the program.
The software does not include any modifications to the QT 
source code and uses the library as is. More information 
about what licensing schemes are used in QT is available [here](https://doc.qt.io/qt-5/licenses-used-in-qt.html). 
QT source code can be acquired by following instructions [here](https://wiki.qt.io/Building_Qt_5_from_Git#Getting_the_source_code).