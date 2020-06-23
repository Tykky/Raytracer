
.. _program_listing_file_Source_materials_Dielectric.h:

Program Listing for File Dielectric.h
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Dielectric.h>` (``Source\materials\Dielectric.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_DIELECTRIC_H
   #define RAYTRACER_DIELECTRIC_H
   
   #include "Material.h"
   
   class Dielectric : public Material {
   
   private:
       const float ior;
   
   public:
       Dielectric(float ior);
   
       virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                            std::function<float()> &randomFloat) const;
   };
   
   
   #endif //RAYTRACER_DIELECTRIC_H
