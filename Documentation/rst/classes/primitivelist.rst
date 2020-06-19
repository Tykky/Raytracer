Primitivelist class
------------------------

Description
^^^^^^^^^^^^

Primitivelist is a data structure for keeping track of all objects in the world. 
This inherits the abstract primitive class, meaning Primitivelist has hit() member 
function. When this member function is called, all of the objects in this list are 
tested against ray r (given as parameted in hit()) and the closest hit to the camera 
is returned. This enables multiple objects in the world.


API documentation
^^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Primitivelist
    :members:
    :undoc-members:

