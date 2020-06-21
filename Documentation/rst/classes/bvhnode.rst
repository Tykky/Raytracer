Bvhnode class
-------------

Description
^^^^^^^^^^^^

Bounding volume hierarchy (BVH) is a data structure for keeping track of all objects in the world. This is used for same purpose as
Primitivelist but is highly optimized compared to the regular Primitivelist. Objects in the world are
grouped by bounding boxes e.g 10 spheres might be inside one bounding box. When we want to check if 
ray hits any of these objects, we can first check if it hits the bounding box. 
If it doesn't we can safely ignore computing all of the objects inside the bounding box.

.. image:: https://upload.wikimedia.org/wikipedia/commons/2/2a/Example_of_bounding_volume_hierarchy.svg

A picture from wikipedia demonstrates this idea quite well

This method is much faster in terms of time complexity. Assume that division into bounding boxes is done efficiently, 
then time complexity will be :math:`O(\log(n))`.

Following is done to form the actual tree (in order)

1. Pick randomly axis from (x,y,z)
2. Sort objects along this axis
3. split the axis into two sub trees

.. todo::

    Note that qsort is used to do the sorting. This is default sorting algorithm in C.
    This should be replaced with self implemented one.

.. todo::

    Use smart pointers

API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Bvhnode
    :members:
    :undoc-members: