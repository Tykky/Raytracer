# Project Implementation

## Overview
Technically the implemented algorithm corresponds more closely to _path tracer_ than ray tracers.The main difference is that path tracing uses 
random sampling and ray tracing does not. Path tracer will 
produce noise when used with low sample count. The more samples are used the more accurate (less noise) the final output will be. This type of approach is often referred as the [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method).

The project is done without any external dependencies (excluding googletests). 

## Classes, functions and features

### Camera class

The camera consist of eye and screen. Rays 
are traversed from eye to screen and then out 
to the world. The screen corresponds to the 
rendered image. Each pixel on the screen is mapped to a ray which intersects the eye and the pixel it's called from.

![](./data/sketches/camera.svg)
The sketch shows how the trigonometric formulas used in the implementation are derived. 


## Sources

[Ray tracing (graphics), wikipedia](https://en.wikipedia.org/wiki/Ray_tracing_(graphics))

[Path Tracing, wikipedia](https://en.wikipedia.org/wiki/Path_tracing)

[Bounding volume hierarchy, wikipedia](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)

[Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method)

[Möller-trumbore intersection algorithm, wikipedia](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm)

[Cost Analysis of a Ray Tracing algorithm, Bruce Walter and Peter Shirley, July 23, 1997](https://www.graphics.cornell.edu/~bjw/mca.pdf)

[Ray Tracing In One Weekend - The Book Series, Peter Shirley](https://raytracing.github.io/)