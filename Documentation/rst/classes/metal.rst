Metal class
-------------------------

Description
^^^^^^^^^^^^

Metal material is highly reflective material. Rays are mirrored using 
surface normal as axis of symmetry. Blur can be added to reflections by 
adding slight random variation to the reflecting ray.

A vector :math:`\vec{v} \in \mathbb{R}^3` can be mirrored/reflected if surface 
normal :math:`\vec{n} \in \mathbb{R}^3` is known:

.. math::

    \vec{v}_\text{reflect} = \vec{v} - 2 (\vec{v} \cdot \vec{n}) \vec{n}


API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Metal
    :members:
    :undoc-members: