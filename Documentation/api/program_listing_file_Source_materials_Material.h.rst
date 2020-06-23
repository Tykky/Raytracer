
.. _program_listing_file_Source_materials_Material.h:

Program Listing for File Material.h
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_materials_Material.h>` (``Source\materials\Material.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_MATERIAL_H
   #define RAYTRACER_MATERIAL_H
   
   #include "core/Ray.h"
   #include "primitives/Primitive.h"
   #include <functional>
   
   class Material {
   public:
       virtual bool scatter(const Ray &r, const hitrecord &record, Vector3D &attenuation, Ray &scatter,
                            std::function<float()> &randomFloat) const = 0;
   };
   
   
   #endif //RAYTRACER_MATERIAL_H
