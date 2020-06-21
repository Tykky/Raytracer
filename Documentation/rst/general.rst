Overview
########################

The project relies heavily on the book series `Ray Tracing In One Weekend <https://raytracing.github.io/>`_ 
by Peter Shirley, while other sources are used too, this is the main source
for concepts used in this renderer. 

The project uses OpenGL for displaying the pixels on screen, 
`GLFW <https://github.com/glfw/glfw>`_ is used to manage the OpenGL context and 
spawn window in different operating systems. `GLEW <https://github.com/Perlmint/glew-cmake>`_ 
is used to managed OpenGL extensions in a cross-platform way. 

.. note::
    OpenGL capable graphics card is required to run and build the project with GUI.
    If you don't have one, make sure to build the project with CMake option -DBUILD_GUI=OFF

Parallelization of the raytracing is done using `OpenMP <https://www.openmp.org/>`_.

.. note::
    OpenMP capable compiler is required in order to the parallelization to work. 
    The program should compile in single threaded mode if OpenMP is not available.

Building the project
######################

If your system has all the required depedencies you should be
able to clone and build the project with:

.. code-block:: bash

    git clone https://github.com/Tykky/Raytracer --recursive
    cd raytracer
    mkdir build
    cd build
    cmake ..
    cmake --build . --config release

After that the executables can be found in

.. code-block::

    raytracer/build/bin

The project can be built without GUI by using

.. code-block:: bash

    cmake -DBUILD_GUI=OFF ..

and without tests by using

.. code-block:: bash

    cmake -DBUILD_TEST=OFF ..


System depedencies
===================

* `Google Test <https://github.com/google/googletest>`_ (via git submodule)
* `GLFW <https://github.com/glfw/glfw>`_ (via git submodule)
* `GLEW <https://github.com/nigels-com/glew>`_ (via git submodule)
* `OpenGL <https://www.opengl.org/>`_

Build depedencies 
===================
* `CMake <https://cmake.org/>`_
* `OpenMP <https://www.openmp.org/>`_ capable C++ compiler

Supported compilers
====================
* GCC (on Linux)
* MSVC (on Windows)

Building the documentation
#############################

The documentation is built with Doxygen and Sphinx. 
Doxygen is used to produce API documentation from 
comments in the source code (which produces xml files)
and Sphinx is used to read the xml and produce 
the documentation. Breathe is extension to Sphinx which 
makes Sphinx understand Doxygen xml output. 

Navigate to Documents folder

.. code-block::

    cd Documentation

Generate doxygen xml files

.. code-block::

    doxygen .doxygen.conf

Generate sphinx html files (Linux)

.. code-block::

    make html

Generate sphinx html files (Windows)

.. code-block::

    .\make.bat html

Depedencies
============
* `Doxygen <https://www.doxygen.nl/index.html>`_
* `Sphinx <https://en.wikipedia.org/wiki/Sphinx>`_
* `Breathe <https://github.com/michaeljones/breathe>`_
* `sphinx_rtd_theme <https://github.com/readthedocs/sphinx_rtd_theme>`_


Sources
########

* `Ray tracing (graphics), wikipedia <https://en.wikipedia.org/wiki/Ray_tracing_(graphics)>`_
* `Path Tracing, wikipedia <https://en.wikipedia.org/wiki/Path_tracing>`_
* `Bounding volume hierarchy, wikipedia <https://en.wikipedia.org/wiki/Bounding_volume_hierarchy>`_
* `Monte Carlo method <https://en.wikipedia.org/wiki/Monte_Carlo_method>`__
* `Möller-trumbore intersection algorithm, wikipedia <https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm>`_
* `Cost Analysis of a Ray Tracing algorithm, Bruce Walter and Peter Shirley, July 23, 1997 <https://www.graphics.cornell.edu/~bjw/mca.pdf>`_
* `Ray Tracing In One Weekend - The Book Series, Peter Shirley <https://raytracing.github.io/>`_
* `Snell's law, wikipedia <https://en.wikipedia.org/wiki/Snell%27s_law>`_
* `Schlick's approximation, wikipedia <https://en.wikipedia.org/wiki/Schlick%27s_approximation>`_
* `Moissanite, wikipedia <https://en.wikipedia.org/wiki/Moissanite>`_
* `Refractive index, wikipedia <https://en.wikipedia.org/wiki/Refractive_index>`_
* `Photon energy, wikipedia <https://en.wikipedia.org/wiki/Photon_energy>`_
* `Fresnel equations, wikipedia <https://en.wikipedia.org/wiki/Fresnel_equations>`_
* `Programming parallel computers, Aalto university <http://ppc.cs.aalto.fi/>`_