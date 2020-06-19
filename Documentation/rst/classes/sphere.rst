Sphere class
-------------

Description
^^^^^^^^^^^^

Sphere class defines the sphere primitive. This inherits the abstract primitive 
class and therefore defines the hit member function. The ray intersection equation 
for the sphere is defined as

.. math::

    (\hat{p} - \hat{c}) \cdot (\hat{p}-\hat{c}) = r^2

where :math:`\hat{c} \in \mathbb{R}^{3}` is 
the origin of the sphere and :math:`r \in \mathbb{R}` 
is the radius. The equation states that 
if point :math:`\hat{p}` satisfies the equation then 
this point is on the sphere. We want to find out
which point(s) :math:`\hat{p}` along the 
`ray <ray.rst>`_ satisfy 
this equation (if any). So we need to solve which 
:math:`c \in \mathbb{R}` satisfies

.. math::

    (\text{ray}(c) - \hat{c}) \cdot (\text{ray}(c) - \hat{c}) = r^2. 

Expanding the equation results in

.. math::

    (\hat{o} + c \cdot \hat{d} - \hat{c}) \cdot (\hat{o} + c \cdot \hat{d} - \hat{c}) = r^2

and expanding the equation further results in 

.. math::

    c^2 \hat{d} \cdot \hat{d} + 2c\hat{d} \cdot (\hat{o} - \vec{c}) + (\hat{o} - \hat{c}) \cdot (\hat{o} - \hat{c}) - r^2 = 0

which can be solved with the quadratic formula

.. math::

    c = \frac{ -2\hat{d} \cdot (\hat{o}-c) \pm \sqrt{(2\hat{d} \cdot (\hat{o}-c))^2 - 4 \hat{d} \cdot \hat{d}((\hat{o} - \hat{c}) \cdot (\hat{o} - \hat{c}) - r^2) }) }
    { 2 \hat{d} \cdot \hat{d} }.


API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Sphere
    :members:
    :undoc-members: