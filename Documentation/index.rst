########################
Raytracer documentation
########################

This project is done for University of Helsinki's course 
*Data structures project*. The course has ended but 
development of this project continues.

The project relies heavily on the mini book series `Ray Tracing In One Weekend <https://raytracing.github.io/>`_ 
by Peter Shirley, while other sources are used too, this is the main source
for concepts used in this renderer. 

Features added during the course
===================================
* Core functionality: Camera, ray, vector, sampling and recursive ray scattering
* Primitives: Sphere 
* Materials: Lambertian, metal and dielectric
* Data structures: BVH and primitivelist
* Simple command line interface
* Parallelization with OpenMP

Features added after the course
=================================
* OpenGL GUI
* MSVC support
* Sphinx documentation

##################
Table of contents
##################

.. toctree::
   rst/general
   :caption: General
   
.. toctree::
   rst/tests
   :caption: Tests

.. toctree::
   ../api/raytracer_root
   :maxdepth: 1
   :caption: API Reference
