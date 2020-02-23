#include "gtest/gtest.h"
#include "materials/Lambertian.h"
#include <random>
#include <functional>

using namespace std;

/** @brief Googletest fixture for testing Lambertian material. */
class lambertiantest : public ::testing::Test {
protected:

    Material *mat;
    mt19937 engine;
    uniform_real_distribution<float> dist;
    function<float()> randomfloat;

    virtual void SetUp() {
        mat = new Lambertian(vector3D(0.5, 0.5, 0.5));
        engine = mt19937(1337);
        dist = uniform_real_distribution<float>(0, 1);
        randomfloat = bind(dist, engine);
    }
};


TEST_F(lambertiantest, scattertest) {

    hitrecord record;
    record.normal = vector3D(-1, 1, 0);
    record.p = vector3D(1, 1, 0);
    record.c = 1;

    vector3D attenuation;
    Ray scatter;
    EXPECT_EQ(true, mat->scatter(Ray(), record, attenuation, scatter, randomfloat));

    EXPECT_EQ(attenuation.getX(), 0.5);
    EXPECT_EQ(attenuation.getY(), 0.5);
    EXPECT_EQ(attenuation.getZ(), 0.5);

    vector3D scat = scatter.getDirection() - scatter.getOrigin();
    // If vector length < 1, the point is inside unit sphere.
    EXPECT_LE((record.normal - scatter.getDirection()).length(), 1);


}