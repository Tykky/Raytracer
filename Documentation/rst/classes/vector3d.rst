Vector3D class
-----------------

Vector3D class contains data structure for 3 dimensional euclidean and 
RGB vectors. This includes arithmetic operations between Vector3D objects. 
The whole vector class is inlined to improve performance. Majority of the 
operations done by the algorithm are done using vectors. Hence these 
are called millions of times in a short timespan. Therefore operation call
overhead causes significant impact.

.. doxygenclass:: Vector3D
    :members:
    :undoc-members: