# Project Specification

In this project I'm going to implement fully working [Raytracer](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)). 

Programming language (for now) is C++. This might change to Java early on if unit testing and test coverage reports are unnecessarily hard to manage when using C++.

## Algorithms & datastructures

## Overview of the implementation

The algorithm is used to produce visual images from 3D environments. It can simulate variety of optical effect within the 3D enviroment, such as reflection, refraction, indirect lightning and shadows.

[Ray tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)) can be implemented in several different ways. Some of the design choices affect the efficiency and time complexity of the overall algorithm.

### sub-algorithms to be implemented
* Matrix multiplication ([Strassen algorithm](https://en.wikipedia.org/wiki/Strassen_algorithm))
* Vector dot product, sum etc
* [Möller-trumbore intersection algorithm](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm)

## Data structures to be implemented

* [Bounding volume hierarchy](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy) or [k-d tree](https://en.wikipedia.org/wiki/K-d_tree)

* ArrayList/STL vector

## References

[Ray tracing (graphics), wikipedia](https://en.wikipedia.org/wiki/Ray_tracing_(graphics))

[Bounding volume hierarchy, wikipedia](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)

[k-d tree, wikipedia](https://en.wikipedia.org/wiki/K-d_tree)

[Möller-trumbore intersection algorithm, wikipedia](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm)


[Cost Analysis of a Ray Tracing algorithm, Bruce Walter and Peter Shirley, July 23, 1997](https://www.graphics.cornell.edu/~bjw/mca.pdf)