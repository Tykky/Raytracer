
.. _program_listing_file_Source_primitives_Bvhnode.h:

Program Listing for File Bvhnode.h
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Bvhnode.h>` (``Source\primitives\Bvhnode.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef RAYTRACER_BVHNODE_H
   #define RAYTRACER_BVHNODE_H
   
   #include "primitives/Primitive.h"
   #include <functional>
   
   // TODO: refactor box comparators (less copy & paste)
   
   int boxCompareX(const void *a, const void *b);
   int boxCompareY(const void *a, const void *b);
   int boxCompareZ(const void *a, const void *b);
   
   class Bvhnode : public Primitive {
   
   private:
       Primitive *pLeft;
       Primitive *pRight;
       Aabb node;
       
   public:
       Bvhnode();
       Bvhnode(Primitive **list, int n, float c0, float c1, std::function<float()> &randomFloat);
       virtual bool hit(const Ray &r, float cmin, float cmax, hitrecord &record) const;
       virtual bool boundingBox(float c0, float c1, Aabb &box) const;
   
   };
   
   
   #endif //RAYTRACER_BVHNODE_H
