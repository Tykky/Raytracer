#include "gtest/gtest.h"
#include "materials/Dielectric.h"
#include <random>
#include <functional>

class DielectricTest : public ::testing::Test {
protected:

    Material *mat;
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist;
    std::function<float()> randomfloat;

    virtual void SetUp() {
        mat = new Dielectric(1.3);
        engine = std::mt19937(1337);
        dist = std::uniform_real_distribution<float>(0, 1);
        randomfloat = bind(dist, engine);
    }

    virtual void TearDown() {
        delete mat;
    }
};

TEST_F(DielectricTest, scattertest) {

    hitrecord record;
    record.normal = Vector3D(0, 1, 0);
    record.p = Vector3D(0, 2, 0);
    record.c = 1;

    Vector3D attenuation;
    Ray scatter;
    Ray r(Vector3D(-2,4,0),Vector3D(0,2,0));

    mat->scatter(r,record,attenuation,scatter,randomfloat);

    EXPECT_FLOAT_EQ(0,scatter.getOrigin().getX());
    EXPECT_FLOAT_EQ(2,scatter.getOrigin().getY());
    EXPECT_FLOAT_EQ(0,scatter.getOrigin().getZ());

    EXPECT_FLOAT_EQ(0,scatter.getDirection().getX());
    EXPECT_FLOAT_EQ(1,scatter.getDirection().getY());
    EXPECT_FLOAT_EQ(0, scatter.getDirection().getZ());

}
