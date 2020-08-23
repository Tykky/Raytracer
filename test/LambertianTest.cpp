#include "gtest/gtest.h"
#include "materials/Lambertian.h"
#include <random>
#include <functional>

class LambertianTest : public ::testing::Test {
protected:

    Material *mat;
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist;
    std::function<float()> randomfloat;

    virtual void SetUp() {
        mat = new Lambertian(Vector3D(0.5, 0.5, 0.5));
        engine = std::mt19937(1337);
        dist = std::uniform_real_distribution<float>(0, 1);
        randomfloat = bind(dist, engine);
    }

    virtual void TearDown() {
        delete mat;
    }

};


TEST_F(LambertianTest, scattertest) {

    Hitrecord record;
    record.normal = Vector3D(-1, 1, 0);
    record.p = Vector3D(1, 1, 0);
    record.distance = 1;

    Vector3D attenuation;
    Ray scatter;
    EXPECT_EQ(true, mat->scatter(Ray(), record, attenuation, scatter, randomfloat));

    EXPECT_EQ(attenuation.getX(), 0.5);
    EXPECT_EQ(attenuation.getY(), 0.5);
    EXPECT_EQ(attenuation.getZ(), 0.5);

    Vector3D scat = scatter.getDirection() - scatter.getPosition();
    // If vector length < 1, the point is inside unit sphere.
    EXPECT_LE((record.normal - scatter.getDirection()).length(), 1);


}