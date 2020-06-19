Abstract primitive class
--------------------------

Description
^^^^^^^^^^^^
Abstract primitive class or just Primitive 
class provides interface for all "primitives". 
Primitives are objects which cannot 
be divided into smaller pieces e.g triangles, 
polygons or spheres. All of the classes which 
inherit this class define the hit member function. 
Hit member function solves ray intersection equation. 
Note that the ray intersection equation is different 
for each primitive

API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Primitive
    :members:
    :undoc-members: