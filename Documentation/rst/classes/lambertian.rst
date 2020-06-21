Lambertian class
-------------

Description
^^^^^^^^^^^^

Lambertian material is diffusively reflecting or "matte" material. 
Rays are reflected to random directions. The real mathematical 
representation of Lambertian (`Lambert's cosine law <https://en.wikipedia.org/wiki/Lambert%27s_cosine_law>`_) 
does not scatter rays by random hence this implementation is only a approximation.

API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Lambertian
    :members:
    :undoc-members: