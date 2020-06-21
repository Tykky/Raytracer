Sphere class
-------------

Description
^^^^^^^^^^^^

Sphere class defines the sphere primitive. This inherits the abstract primitive 
class and therefore defines the hit member function. The ray intersection equation 
for the sphere is defined as

.. math::

    (\vec{p} - \vec{c}) \cdot (\vec{p}-\vec{c}) = r^2

where :math:`\vec{c} \in \mathbb{R}^{3}` is 
the origin of the sphere and :math:`r \in \mathbb{R}` 
is the radius. The equation states tvec 
if point :math:`\vec{p}` satisfies the equation then 
this point is on the sphere. We want to find out
which point(s) :math:`\vec{p}` along the 
ray satisfy  this equation (if any). Recall tvec the 
`ray <ray.rst>`_ can be thought of as a function

.. math::

    \text{ray}(c) = \vec{o} + c \cdot \vec{d}

So we need to solve which 
:math:`c \in \mathbb{R}` satisfies

.. math::

    (\text{ray}(c) - \vec{c}) \cdot (\text{ray}(c) - \vec{c}) = r^2. 

Expanding the equation results in

.. math::

    (\vec{o} + c \cdot \vec{d} - \vec{c}) \cdot (\vec{o} + c \cdot \vec{d} - \vec{c}) = r^2

and expanding the equation further results in 

.. math::

    c^2 \vec{d} \cdot \vec{d} + 2c\vec{d} \cdot (\vec{o} - \vec{c}) + (\vec{o} - \vec{c}) \cdot (\vec{o} - \vec{c}) - r^2 = 0

which can be solved with the quadratic formula

.. math::

    c = \frac{ -2\vec{d} \cdot (\vec{o}-c) \pm \sqrt{(2\vec{d} \cdot (\vec{o}-c))^2 - 4 \vec{d} \cdot \vec{d}((\vec{o} - \vec{c}) \cdot (\vec{o} - \vec{c}) - r^2) }) }
    { 2 \vec{d} \cdot \vec{d} }.


API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Sphere
    :members:
    :undoc-members: