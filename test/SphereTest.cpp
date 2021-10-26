#include "gtest/gtest.h"
#include <memory>
#include "hittables/Sphere.h"
#include "materials/Lambertian.h"

class SphereTest : public ::testing::Test
{
protected:

    std::unique_ptr<Hittable> sphr;
    std::unique_ptr<Material> lamb;

    virtual void SetUp()
    {
        lamb = std::make_unique<Lambertian>(Vector3D(1,1,1));
        sphr = std::make_unique<Sphere>(Vector3D(0,0,0), 1, lamb.get());
    }
};