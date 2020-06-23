
.. _program_listing_file_Source_primitives_Primitivelist.cpp:

Program Listing for File Primitivelist.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_Source_primitives_Primitivelist.cpp>` (``Source\primitives\Primitivelist.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Primitivelist.h"
   #include "core/Utility.h"
   
   Primitivelist::Primitivelist(Primitive **l, int n) :
           size(n), list(l) {
   }
   
   bool Primitivelist::hit(const Ray &r, float cmin, float cmax, hitrecord &record) const {
   
       bool hit = false;
       hitrecord tmprec;
       float closest = cmax;
   
       // Test Ray r against all primitives
       for (int i = 0; i < size; ++i) {
           // Preserve only the closest hit
           if (list[i]->hit(r, cmin, closest, tmprec)) {
               hit = true;
               closest = tmprec.c;
               record = tmprec;
           }
       }
       return hit;
   }
   
   bool Primitivelist::boundingBox(float c0, float c1, Aabb &box) const {
       if(size < 1) return false;
   
       Aabb tmpbox;
       bool first = list[0]->boundingBox(c0,c1,tmpbox);
   
       if(!first) return false;
   
       box = tmpbox;
       for (int i = 1; i < size; ++i) {
           if(list[i]->boundingBox(c0,c1,tmpbox)) {
               box = surroundingBox(box,tmpbox);
           } else {
               return false;
           }
       }
       return true;
   }
