#include "gtest/gtest.h"
#include "core/Ray.h"

TEST(RayTest, getPointAtCtest) {
    Ray r = Ray(Vector3D(1, 1, 1), Vector3D(3, 3, 3));
    Vector3D c = r.pointAtDistance(0.5);
    EXPECT_FLOAT_EQ(2.5, c.getX());
    EXPECT_FLOAT_EQ(2.5, c.getY());
    EXPECT_FLOAT_EQ(2.5, c.getZ());
}