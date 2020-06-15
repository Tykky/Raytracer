Camera class
-------------

The camera consist of eye and screen. Rays 
are traversed from eye to screen and then out 
to the world. The screen corresponds to the 
rendered image. Each pixel on the screen is mapped to a ray which intersects the eye and the pixel it's called from.

.. image:: ../../data/sketches/camera.svg
The sketch shows how the trigonometric formulas used in the implementation are derived. 


.. doxygenclass:: Camera
    :members:
