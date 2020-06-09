#include "materials/Material.h"
#include "materials/Metal.h"
#include "gtest/gtest.h"
#include "Ray.h"
#include <random>

/** @brief Googletest fixture for testing Metal material. */
class MetalTest : public ::testing::Test {
protected:

    Material *mat;
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist;
    std::function<float()> randomfloat;

    void SetUp() {
        engine = std::mt19937(1337);
        dist = std::uniform_real_distribution<float>(0, 1);
        randomfloat = bind(dist, engine);
        mat = new Metal(Vector3D(0.5,0.5,0.5),0);
    }

    void TearDown() {
        delete mat;
    }
};

TEST_F(MetalTest, scattertest) {

    hitrecord record;
    record.p = Vector3D(0,2,0);
    record.normal = Vector3D(0,-1,0);
    record.c = 1;
    record.matptr = mat;

    Ray r = Ray(Vector3D(-2,0,0),Vector3D(0,2,0));
    Vector3D att(1,1,1);
    Ray scat = Ray();

    mat->scatter(r,record,att,scat,randomfloat);

    EXPECT_EQ(0,scat.getOrigin().getX());
    EXPECT_EQ(2,scat.getOrigin().getY());
    EXPECT_EQ(0,scat.getOrigin().getZ());

    EXPECT_EQ(0,scat.getDirection().getX());
    EXPECT_EQ(-2,scat.getDirection().getY());
    EXPECT_EQ(0,scat.getDirection().getZ());


}