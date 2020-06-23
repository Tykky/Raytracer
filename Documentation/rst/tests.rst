Unit tests
############

Unit testing is done with `googletest <https://github.com/google/googletest>`_ and 
project is automatically build and tested at `Travis CI <https://travis-ci.org/>`_. 
Test coverage reports are generated with gcc (using -coverage flag). 
After testing has finished the reports are uploaded to 
`codecov.io <https://codecov.io/>`_ from Travis. 

Unit tests that are currently implemented are 
straightforward. Most of them use pre-calculated 
values which are then compared against the code.

BVH vs primitivelist
######################

The time complexity of bvh is :math:`O(\log(n))` and for the primitivelist it is :math:`O(n)`. Now this 
was tested by measuring render times in the demo scene by varying the amount of 
randomly generated spheres. Resolution of 200x100 was used to reduce render times
to more feasible figures. 

.. image:: ../data/comp.png

Graph shows difference between :math:`O(\log(n))` and :math:`O(n)` implementation. 

.. image:: ../data/renders/demo.png

A picture of the used demo scene. At the picture the demo scene has slightly over 1000 spheres.