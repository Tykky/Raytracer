
.. _program_listing_file_Source_materials_Lambertian.cpp:

Program Listing for File Lambertian.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Lambertian.cpp>` (``Source\materials\Lambertian.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Lambertian.h"
   #include "core/Utility.h"
   
   Lambertian::Lambertian(const Vector3D &albedo) :
           albedo(albedo) {
   }
   
   bool Lambertian::scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                            std::function<float()> &randomFloat) const {
       Vector3D rpoint = record.p + record.normal + randomInUnitSphere(randomFloat);
       scatter = Ray(record.p, rpoint - record.p);
       attenuation = albedo;
       return true;
   }
