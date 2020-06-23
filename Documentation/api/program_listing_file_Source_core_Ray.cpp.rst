
.. _program_listing_file_Source_core_Ray.cpp:

Program Listing for File Ray.cpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_core_Ray.cpp>` (``Source\core\Ray.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Ray.h"
   
   Ray::Ray() :
       origin({}), direction({}) {
   };
   
   Ray::Ray(const Vector3D& origin, const Vector3D& direction) :
       origin(origin), direction(direction) {
   };
   
   Vector3D Ray::getOrigin() const {
       return origin;
   }
   
   Vector3D Ray::getDirection() const {
       return direction;
   }
   
   Vector3D Ray::pointAtC(float c) const {
       return origin + c * direction;
   }
