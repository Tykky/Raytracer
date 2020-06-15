Ray class
-------------

The ray class handles mapping between two Vector3D vectors. These two vectors 
are named **origin** and **direction**. While regular euclidean vectors have length 
and direction, the ray has position, length and direction. The ray class has 
a member function to return arbitrary point along it's span. This feature is used 
to solve the hit equation or ray intersection equation. Note that The ray intersection equation 
is different for each primitive. 


.. doxygenclass:: Ray
    :members: