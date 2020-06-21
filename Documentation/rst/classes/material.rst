Abstract material class
-------------------------

Description
^^^^^^^^^^^^

Abstract material class works similarly to abstract primitive class. 
This defines interface for all materials in the world. All materials 
have scatter() member function which determines direction for a ray 
when it bounces from surface with this material. Note that some 
materials can let ray to pass through surface. Materials and primitives 
are closely linked since computing a scatter/bounce ray requires 
information about the surface it hits and the material.


API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Material
    :members:
    :undoc-members: