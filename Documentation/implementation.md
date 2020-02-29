# Project Implementation

## Overview
Technically the implemented algorithm corresponds more closely to _path tracer_ than ray tracers.The main difference is that path tracing uses 
random sampling and ray tracing does not. Path tracer will 
produce noise when used with low sample count. The more samples are used the more accurate (less noise) the final output will be. This type of approach is often referred as the [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method).

The project is done without any external dependencies (excluding googletests). 

## Classes

### Vector3D

Vector3D class contains data structure for 3 dimensional euclidean and 
RGB vectors. This includes arithmetic operations between Vector3D objects. 
The whole vector class is inlined to improve performance. Majority of the 
operations done by the algorithm are done using vectors. Hence these 
are called millions of times in a short timespan. Therefore operation call
overhead causes significant impact. 

### Ray

The ray class handles mapping between two Vector3D vectors. These two vectors 
are named origin and direction. While regular euclidean vectors have length 
and direction. The ray has position, length and direction. The ray class has 
member function to return arbitrary point along it's span. This feature is used to 
solve the hit equation or ray intersection equation. 

### Camera

The camera consist of eye and screen. Rays 
are traversed from eye to screen and then out 
to the world. The screen corresponds to the 
rendered image. Each pixel on the screen is mapped to a ray which intersects the eye and the pixel it's called from.

![](./data/sketches/camera.svg)
The sketch shows how the trigonometric formulas used in the implementation are derived. 

### Engine

The engine class is used to do the actual rendering process. The _screen_
(in camera sketch) is traversed with nested for loops. The parallelization 
is done by splitting the nested for loop into segments (rectangles on screen) and assigning each segment to a separate thread. Each thread processes then 
pixels on it's own segment in parallel. Implementing the paralellization this 
way causes no critical sections in the code. 

Each pixel inside each segment is mapped to a ray and then the ray is send out to the world to see how it bounces from different objects. Color for each pixel is 
computed based on these bounces. This mapping is done multiple times 
using slight variation in (x,y) screen coordinate and then the 
color from these _samples_ is averaged. This is called random [supersampling](https://en.wikipedia.org/wiki/Supersampling). The idea is to remove jagged edges from the 
final image. 

Now these samples are also used to produce scatter rays. Rays can 
be set to bounce in random directions or set to bounce by probability distribution. 
Meaning rays will bounce differently on each sample. This is helpful when 
trying to simulate [global illumination](https://en.wikipedia.org/wiki/Global_illumination). When probability distribution is used to distribute light the term [Monte Carlo Path tracing](https://en.wikipedia.org/wiki/Path_tracing) or just path tracing is used instead of raytracing. The method produces realistic lighting.

Downside of this method is that the final image is usually noisy. Increasing sample 
count can be used to combat this issue but with diminishing returns. Another popular method to combat this is to use denoise algorithm which isn't implemented in this project. 

### Primitives


#### Abstract primitive

Abstract primitive class or just Primitive class 
provides interface for all "primitives". Primitives are objects in the scene which cannot be divided into smaller pieces e.g triangles, polygons or spheres. All of 
the classes which inherit this class must have "hit" member function. Hit member 
function solves the hit equation or ray intersection equation. The hit equation for is 
different for each primitive. 

### Sphere

Sphere class defines the sphere primitive. This inherits the abstract primitive 
class. The hit equation for the sphere is formulated using vectors:
<img src="https://latex.codecogs.com/svg.latex?(r(c)-\text{center})\cdot
(r(c)-\text{center}) = R^2" title="(r(c)-\text{center})\cdot
(r(c)-\text{center}"/>


## Sources

[Ray tracing (graphics), wikipedia](https://en.wikipedia.org/wiki/Ray_tracing_(graphics))

[Path Tracing, wikipedia](https://en.wikipedia.org/wiki/Path_tracing)

[Bounding volume hierarchy, wikipedia](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)

[Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method)

[Möller-trumbore intersection algorithm, wikipedia](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm)

[Cost Analysis of a Ray Tracing algorithm, Bruce Walter and Peter Shirley, July 23, 1997](https://www.graphics.cornell.edu/~bjw/mca.pdf)

[Ray Tracing In One Weekend - The Book Series, Peter Shirley](https://raytracing.github.io/)