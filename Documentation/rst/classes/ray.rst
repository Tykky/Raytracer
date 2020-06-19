Ray class
-------------

Description
^^^^^^^^^^^^

The ray class handles mapping between two Vector3D vectors. These two vectors 
are named **origin** and **direction**. While regular euclidean vectors have length 
and direction, the ray has position, length and direction.

Mathematically the ray can be thought of as a function

.. math::

    \text{ray}(c) = \hat{o} + c \cdot \hat{d}

where :math:`\hat{o} \in \mathbb{R}^3` is the **origin**, :math:`\hat{d} \in \mathbb{R}^3`
is the **direction** and :math:`c \in \mathbb{R}` is scalar.


API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Ray
    :members:
    :undoc-members: