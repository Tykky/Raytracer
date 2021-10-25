#ifndef RAYTRACER_UTILITY_H
#define RAYTRACER_UTILITY_H

#include "Vector3D.h"
#include "Ray.h"
#include <functional>
#include <cmath>

// Produces white to white blue gradient
Vector3D skyGradient(const Ray& r);

Vector3D randomInUnitSphere(std::function<float()>& randomFloat);
Vector3D reflect(const Vector3D& v, const Vector3D& n);
bool refract(const Vector3D& v, const Vector3D& n, float ior, Vector3D& refraction);
float fresnel(float cosine, float ior);

#endif //RAYTRACER_UTILITY_H
