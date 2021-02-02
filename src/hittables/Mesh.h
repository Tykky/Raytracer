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
    Mesh(std::vector<Triangle> triangles, Material *material) : triangles_(std::move(triangles)), material_(material) {};
    virtual bool hit(const Ray &r, float dmin, float dmax, Hitrecord &record) const;
    virtual bool boundingBox(float c0, float c1, Aabb &box) const;
    const std::vector<Triangle>& getTriangles() { return triangles_; };

private:
    Lambertian default_color = Lambertian({0.58f, 0.f, 0.82f});
    std::vector<Triangle> triangles_ = {};
    Material *material_ = &default_color;
};


#endif // RAYTRACER_MESH_H