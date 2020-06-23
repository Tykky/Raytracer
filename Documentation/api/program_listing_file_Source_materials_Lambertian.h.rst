
.. _program_listing_file_Source_materials_Lambertian.h:

Program Listing for File Lambertian.h
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Lambertian.h>` (``Source\materials\Lambertian.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_LAMBERTIAN_H
   #define RAYTRACER_LAMBERTIAN_H
   
   #include "Material.h"
   #include "functional"
   
   class Lambertian : public Material {
   private:
       const Vector3D albedo;
   public:
       Lambertian(const Vector3D &albedo);
   
       virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                            std::function<float()> &randomFloat) const;
   };
   
   
   #endif //RAYTRACER_LAMBERTIAN_H
