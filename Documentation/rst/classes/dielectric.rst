Dielectric class
-----------------

Description
^^^^^^^^^^^^

Dielectric material allows rays to pass through surfaces. 
`Snell's law <https://en.wikipedia.org/wiki/Snell%27s_law>`_
determines refraction angle when *light or other waves 
travel through a boundary between two different isotropic 
media, such as water, glass or air*. Snell's law is 
often expressed in form

.. math::

    \frac{ \sin{\theta_2} }{ \sin{\theta_1} } = \frac{v_2}{v_1} = \frac{n_1}{n_2}

where :math:`\theta_1` is the angle where light enters boundary and :math:`\theta_2`
is the angle where lights exits the boundary. :math:`v_1`, :math:`v_2` are 
velocities and :math:`n_1`, :math:`n_2` are indices of refraction respectively. 
The renderer defines rays using vectors and therefore the vector form

.. math::

    \vec{v}_\text{refract} = r\vec{l} + \left(rc - \sqrt{1-r^2(1-c^2)}\right)\vec{n}

is more useful where :math:`r=\frac{n_1}{n_2}`, :math:`c=-\vec{n} \cdot \vec{l}` 
and :math:`\vec{l}` is a vector that is corresponds to the incoming ray.

.. img:: https://upload.wikimedia.org/wikipedia/commons/5/5d/RefractionReflextion.svg

API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Dielectric
    :members:
    :undoc-members:
