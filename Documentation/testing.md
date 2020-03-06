# Project Testing


## Unit testing 

Unit testing is done with [googletest](https://github.com/google/googletest) and project is automatically build and tested at [Travis CI](https://travis-ci.org/). Test coverage reports are generated with gcc (using -coverage flag). After testing has finished the reports are upload to [codecov.io](https://codecov.io/) from Travis. 

Unit tests that are currently implemented are 
straight forward. Most of them use pre calculated 
values which are then compared against the code.


### Vector3DTest

Vector operations are tested with pre-calculated values. If vector operations work with these simple test cases then it's highly likely 
that they will work with any values. 

### CameraTest

Camera is tested with pre-calculated values. Fov, aspect ratio, origin
and pointat position are set to predefiend values. Then it is tested 
that camera returns correct rays for center and corners of the screen. 

### LambertianTest

The lambertian material is tested by checking if it returns correct 
scatter ray when scatter() function is called. This is done by 
knowing that the ray should be inside unit circle. Vector length 
is measured to check if the vector stays inside this unit circle.


### RayTest

Rays are so simple that only one test is used. It is tested that 
does pointAtC member function return correct values.


