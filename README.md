# Raytracer


This repository contains a raytracer for University of Helsinki's course Data Structures Project.

[![Build Status](https://travis-ci.org/Tykky/Raytracer.svg?branch=master)](https://travis-ci.org/Tykky/Raytracer)
[![codecov](https://codecov.io/gh/Tykky/Raytracer/branch/master/graph/badge.svg)](https://codecov.io/gh/Tykky/Raytracer)
## Documentation

[Project Specification](./Documentation/specification.md)

[Project Implementation](./Documentation/implementation.md)

[Project Testing](./Documentation/testing.md)

[Timesheet](./Documentation/timesheet.md)

[Doxygen doc](https://tykky.github.io/)

## Weekly reports

* [Week 1](./Documentation/reports/week1.md)
* [Week 2](./Documentation/reports/week2.md)
* [Week 3](./Documentation/reports/week3.md)
* [Week 4](./Documentation/reports/week4.md)
* [Week 5](./Documentation/reports/week5.md)
* [Week 6](./Documentation/reports/week6.md)

## How to build on linux
```
git clone --recursive https://github.com/Tykky/Raytracer
cd Raytracer
mkdir build
cd build
cmake ..
make
```
After building the program can be run with:
```
./Source/raytracer
```
Tests can be run with:
```
./Test/gtest_run
```

The project shouldn't have any operating system specific dependencies. This should be compilable on windows (not tested). To be able to build you need:

- cmake 3.12.4 (or newer)
- gcc (9.2 preferably)

## Command-line interface

The program prompts user to enter 

- screen width (image width in pixels)
- screen height (image height in pixels)
- camera preset from 4 different options (0-3)
- fov (vertical field of view in degrees)
- samples (number of times each pixel is sampled)


After rendering is finished, the program should output image to 
the current directory (the same one where program is run).

## Current state of the project

![](Documentation/data/renders/demo.png)
The render is now more efficient in rendering 
multiple objects. This is due to use of bounding 
volume hierarchy data structure. Hit testing is done 
in logarithmic time O(log(n)) instead of linear.
