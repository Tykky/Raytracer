.. Raytracer documentation master file, created by
   sphinx-quickstart on Sat Jun 13 05:53:02 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

##########################################
Raytracer documentation
##########################################

This project is done for University of Helsinki's course 
*Data structures project*. The course has ended but 
development of this project continues. 

Features added during the course
===================================
* Core functionality: Camera, ray, vector, engine and recursive ray scattering
* Primitives: Sphere 
* Materials: Lambertian, Metal, Dielectric and generic BRDF
* Data structures: BVH and primitivelist
* Simple command line interface
* Parallelization with OpenMP

Features added after the course
=================================
* OpenGL GUI
* MSVC support
* Sphinx documentation

####################
Table of contents
####################

.. toctree::
   rst/general
   :caption: General

.. toctree::
   rst/api
   :caption: API reference

.. toctree::
   rst/tests
   :caption: Tests
