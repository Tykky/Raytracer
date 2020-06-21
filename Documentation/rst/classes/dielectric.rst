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

is more useful where :math:`r=\frac{n_1}{n_2}`, :math:`\vec{n}` is surface normal, 
:math:`\vec{l}` is a vector that corresponds to the incoming ray and :math:`c=-\vec{n} \cdot \vec{l}`.

Note that refraction is not always possible. Total internal reflection happens when 
:math:`1-r^2(1-c^2) < 0 \implies r^2(1-c^2) > 1`, meaning the ray should reflect 
instead of refraction. 

.. image:: https://upload.wikimedia.org/wikipedia/commons/5/5d/RefractionReflextion.svg

A picture from wikipedia demonstrates the internal reflection.

Additionally to the Snell's Law, the renderer takes `Fresnel <https://en.wikipedia.org/wiki/Fresnel_equations>`_ 
effect into account. According to wikipedia, *When light strikes the interface between a medium with 
refractive index* :math:`n_1` *and a second medium with refractive index* :math:`n_2`, *both reflection and refraction 
of the light may occur.* Fresnel coefficient(s) define the ratio between reflection and refraction. 
The renderer uses this ratio as probability. 

Instead of computing the ratio using Fresnel equations, the renderer 
uses `Schlick`s approximation <https://en.wikipedia.org/wiki/Schlick%27s_approximation>`_


.. math:: 

    R(\theta)  = R_0 + (1-R_0)(1-\cos(\theta))^5

where :math:`R` is the ratio, :math:`\theta` is the angle and 

.. math::

    R_0 = \left( \frac{ n_1 - n_2 }{ n_1 + n_2 } \right)



API documentation
^^^^^^^^^^^^^^^^^^

.. doxygenclass:: Dielectric
    :members:
    :undoc-members:
