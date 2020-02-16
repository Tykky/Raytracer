#include "gtest/gtest.h"
#include "ray.h"

TEST(raytest, getPointAtCtest) {
    ray r = ray(vector3D(1,1,1),vector3D(3,3,3));
    vector3D c = r.pointAtC(0.5);
    EXPECT_FLOAT_EQ(2.5, c.getX());
    EXPECT_FLOAT_EQ(2.5,c.getY());
    EXPECT_FLOAT_EQ(2.5,c.getZ());
}