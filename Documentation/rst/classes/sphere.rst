Sphere class
-------------

Sphere class defines the sphere primitive. This inherits the abstract primitive 
class and therefore defines the hit function. Hit equation of sphere is defined as

.. math::

    (\text{ray}(c)-\hat{s} \cdot (\text{ray}(c)-\hat{s}) = r

where :math:`s \in \mathbb{R}^3` points to the center of the sphere, 
:math:`` and :math:`\text{ray}(c)` is defined as

.. math::

    \text{ray}(c)



.. math::

.. doxygenclass:: Sphere
    :members: