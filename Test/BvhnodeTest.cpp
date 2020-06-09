#include <primitives/Sphere.h>
#include <materials/Lambertian.h>
#include <primitives/Bvhnode.h>
#include "gtest/gtest.h"
#include <random>

/** @brief Googletest fixture for testing the bvh data structure. */
class BvhnodeTest : public ::testing::Test {
protected:

    Primitive *list[4];
    Material *mat;
    Bvhnode *bvh;
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist;
    std::function<float()> randomfloat;

    void SetUp() {
        engine = std::mt19937(1337);
        dist = std::uniform_real_distribution<float>(0, 1);
        randomfloat = bind(dist, engine);
        mat = new Lambertian(Vector3D(1,1,1));
        list[0] = new Sphere(Vector3D(0,0,0),0.5,mat);
        list[1] = new Sphere(Vector3D(-1,0,0),0.5,mat);
        list[2] = new Sphere(Vector3D(1,0,0),0.5,mat);
        list[3] = new Sphere(Vector3D(0,2,0),0.5,mat);
        bvh = new Bvhnode(list,3,0,1,randomfloat);
    }

    void TearDown() {
        for (int i = 0; i < 3; ++i) {
            delete list[i];
        }
        delete bvh;
        delete mat;
    }
};

TEST_F(BvhnodeTest, hitMiddleTest) {

    Ray r(Vector3D(0,0,-2),Vector3D(0,0,-1));
    hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true, hit);
    EXPECT_FLOAT_EQ(0.5, record.p.getZ());

}

TEST_F(BvhnodeTest, hitLeftTest) {
    Ray r(Vector3D(-1,0,-2),Vector3D(-1,0,-1));
    hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(BvhnodeTest, hitLeftRight) {
    Ray r(Vector3D(1,0,-2),Vector3D(1,0,-1));
    hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(BvhnodeTest, missTest) {
    Ray r(Vector3D(5,0,-2),Vector3D(5,0,-1));
    hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);
    EXPECT_EQ(false,hit);
    EXPECT_EQ(0,record.p.getZ());
}