#include <hittables/Sphere.h>
#include <materials/Lambertian.h>
#include <hittables/Bvhnode.h>
#include "gtest/gtest.h"
#include <random>

class BvhnodeTest : public ::testing::Test {
protected:

    std::vector<std::shared_ptr<Hittable>> list;
    std::shared_ptr<Material> mat;
    std::shared_ptr<Bvhnode> bvh;
    std::mt19937 engine;
    std::uniform_real_distribution<float> dist;
    std::function<float()> randomfloat;

    void SetUp() {
        list = std::vector<std::shared_ptr<Hittable>>(4);
        engine = std::mt19937(1337);
        dist = std::uniform_real_distribution<float>(0, 1);
        randomfloat = bind(dist, engine);
        mat = std::make_shared<Lambertian>(Vector3D(1,1,1));
        list[0] = std::make_unique<Sphere>(Vector3D(0,0,0),0.5,mat.get());
        list[1] = std::make_unique<Sphere>(Vector3D(-1,0,0),0.5,mat.get());
        list[2] = std::make_unique<Sphere>(Vector3D(1,0,0),0.5,mat.get());
        list[3] = std::make_unique<Sphere>(Vector3D(0,2,0),0.5,mat.get());
        bvh = std::make_shared<Bvhnode>(list,0,3,-999,999,randomfloat);
    }
};

TEST_F(BvhnodeTest, hitMiddleTest) {

    Ray r(Vector3D(0,0,-2),Vector3D(0,0,-1));
    Hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true, hit);
    EXPECT_FLOAT_EQ(0.5, record.p.getZ());

}

TEST_F(BvhnodeTest, hitLeftTest) {
    Ray r(Vector3D(-1,0,-2),Vector3D(-1,0,-1));
    Hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(BvhnodeTest, hitLeftRight) {
    Ray r(Vector3D(1,0,-2),Vector3D(1,0,-1));
    Hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(BvhnodeTest, missTest) {
    Ray r(Vector3D(5,0,-2),Vector3D(5,0,-1));
    Hitrecord record;

    bool hit = bvh->hit(r,-9999,99999,record);
    EXPECT_EQ(false,hit);
    EXPECT_EQ(0,record.p.getZ());
}