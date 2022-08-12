#include "gtest/gtest.h"
#include "hittables/Aabb.h"
#include <memory>

class AabbTest : public ::testing::Test
{
protected:

    std::unique_ptr<Aabb> aabb_1;
    std::unique_ptr<Aabb> aabb_2;

    virtual void SetUp()
    {
        // Create unit cubes
        aabb_1 = std::make_unique<Aabb>(Vector3D(0,0,0), Vector3D(1,1,1));
        aabb_2 = std::make_unique<Aabb>(Vector3D(-1,-1,-1), Vector3D(0,0,0));
    }
};

TEST_F(AabbTest, hitMiddleTest)
{
    Ray r(Vector3D(0.5f, 0.5f, -2.f), Vector3D(0,0,1));
    EXPECT_TRUE(aabb_1->hit(r, 0, 99));
}

TEST_F(AabbTest, nearMissTest)
{
    Ray r(Vector3D(-0.1f, 0.f, -12.f), Vector3D(0,0,1));
    EXPECT_FALSE(aabb_1->hit(r, 0, 99));
}

TEST_F(AabbTest, surroundingBoxTest)
{
    Aabb sbox = surroundingBox(*aabb_1, *aabb_2);
    EXPECT_FLOAT_EQ(sbox.getMin().getX(), -1.f);
}