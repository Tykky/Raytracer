#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include <utility>

#include "Hittable.h"
#include "hittables/Triangle.h"
#include "materials/Material.h"
#include "materials/Lambertian.h"

class Mesh : public Hittable {
public:
    Mesh() = default;
    Mesh(std::vector<std::shared_ptr<Hittable>> &triangles) : triangles_(triangles) {};
    virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;
    const std::vector<std::shared_ptr<Hittable>> &getTriangles() { return triangles_; };

private:
    std::vector<std::shared_ptr<Hittable>> triangles_ = {};
};


#endif // RAYTRACER_MESH_H