Engine class
---------------
The engine class is used to do the actual rendering process. The *screen*
(in camera sketch) is traversed with nested for loops. The parallelization 
is done by splitting the nested for loop into segments (rectangles on screen) and 
assigning each segment to a separate thread. Each thread processes then 
pixels on it's own segment in parallel. Implementing the paralellization this 
way causes no critical sections in the code. Library called openMP is used to 
do the parallelization. 

Each pixel inside each segment is mapped to a ray and then the ray is send out to 
the world to see how it bounces from different objects. Color for each pixel is 
computed based on these bounces. This mapping is done multiple times 
using slight variation in :math:`(x,y)` screen coordinate and then the 
color from these *samples* is averaged. This is called random 
`supersampling <https://en.wikipedia.org/wiki/Supersampling>`_, the idea is to remove jagged edges from the 
final image. 

Now these samples are also used to produce scatter rays. Rays can 
be set to bounce in random directions or set to bounce by probability distribution. 
Meaning rays will bounce differently on each sample. This is helpful when trying to 
achieve `global illumination <https://en.wikipedia.org/wiki/Global_illumination>`_. 
When probability distribution is used to distribute light the algorithm is
`Path tracer <https://en.wikipedia.org/wiki/Path_tracing>`_ which leverages the 
use of `Monte Carlo method <https://en.wikipedia.org/wiki/Monte_Carlo_method>`_

Downside of this method is that the final image is usually noisy. Increasing sample 
count can be used to combat this issue but with diminishing returns. Another popular 
method to combat this is to use denoise algorithm which isn't implemented in this project.

    
.. doxygenclass:: Engine
    :members:   
