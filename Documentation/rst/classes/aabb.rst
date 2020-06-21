Aabb class
-------------

Description
^^^^^^^^^^^^

The axis-aligned bounding box (AABB)
defines the bounding box that is used by
`BVH <bvhnode.rst>`_. The constructor takes 
two vectors which define 3+3 planes in 3 dimensional 
euclidean space. These planes intersect and 
create a constrained volume or to put it more simply, 
a box.



API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Aabb
    :members:
    :undoc-members: