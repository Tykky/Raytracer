# Project testing


## unit testing 

Unit testing is done with [googletest](https://github.com/google/googletest) and project is automatically build and tested at [Travis CI](https://travis-ci.org/). Test coverage reports are generated with gcc (using -coverage flag). After testing has finished the reports are upload to [codecov.io](https://codecov.io/) from Travis. 

Unit tests that are currently implemented are 
straight forward. Most of them use pre calculated 
values which are then compared against the code.  


