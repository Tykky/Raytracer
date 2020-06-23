#include "gtest/gtest.h"
#include "core/Vector3D.h"

class Vector3DTest : public ::testing::Test {
protected:

    Vector3D v1;
    Vector3D v2;

    virtual void SetUp() {
        v1 = Vector3D(1, 2, 3);
        v2 = Vector3D(3, 4, 5);
    }

};

TEST_F(Vector3DTest, opPlusTest) {
    +(v1);
    EXPECT_FLOAT_EQ(1, v1.getX());
    EXPECT_FLOAT_EQ(2, v1.getY());
    EXPECT_FLOAT_EQ(3, v1.getZ());
}

TEST_F(Vector3DTest, opAddtVectorTest) {
    v1 += v2;
    EXPECT_FLOAT_EQ(4, v1.getX());
    EXPECT_FLOAT_EQ(6, v1.getY());
    EXPECT_FLOAT_EQ(8, v1.getZ());
}

TEST_F(Vector3DTest, opAddConstantTest) {
    float c = 9;
    v1 += c;
    EXPECT_FLOAT_EQ(1 + c, v1.getX());
    EXPECT_FLOAT_EQ(2 + c, v1.getY());
    EXPECT_FLOAT_EQ(3 + c, v1.getZ());
}

TEST_F(Vector3DTest, opMinusTest) {
    Vector3D negv1 = -v1;
    EXPECT_FLOAT_EQ(-1, negv1.getX());
    EXPECT_FLOAT_EQ(-2, negv1.getY());
    EXPECT_FLOAT_EQ(-3, negv1.getZ());
}

TEST_F(Vector3DTest, opDecreaseVectorTest) {
    v1 -= v2;
    EXPECT_FLOAT_EQ(-2, v1.getX());
    EXPECT_FLOAT_EQ(-2, v1.getY());
    EXPECT_FLOAT_EQ(-2, v1.getZ());
}

TEST_F(Vector3DTest, opDecreaseConstantTest) {
    float c = 5;
    v1 -= c;
    EXPECT_FLOAT_EQ(-4, v1.getX());
    EXPECT_FLOAT_EQ(-3, v1.getY());
    EXPECT_FLOAT_EQ(-2, v1.getZ());
}

TEST_F(Vector3DTest, opMultiplyVectorTest) {
    v1 *= v2;
    EXPECT_FLOAT_EQ(3, v1.getX());
    EXPECT_FLOAT_EQ(8, v1.getY());
    EXPECT_FLOAT_EQ(15, v1.getZ());
}

TEST_F(Vector3DTest, opMultiplyConstantTest) {
    float c = 5;
    v1 *= c;
    EXPECT_FLOAT_EQ(5, v1.getX());
    EXPECT_FLOAT_EQ(10, v1.getY());
    EXPECT_FLOAT_EQ(15, v1.getZ());
}

TEST_F(Vector3DTest, opDivideVectorTest) {
    v1 /= v2;
    EXPECT_FLOAT_EQ(.3333333, v1.getX());
    EXPECT_FLOAT_EQ(.5, v1.getY());
    EXPECT_FLOAT_EQ(.6, v1.getZ());
}

TEST_F(Vector3DTest, opDivideConstantTest) {
    float c = 5;
    v1 /= c;
    EXPECT_FLOAT_EQ(0.2, v1.getX());
    EXPECT_FLOAT_EQ(0.4, v1.getY());
    EXPECT_FLOAT_EQ(0.6, v1.getZ());
}

TEST_F(Vector3DTest, lengthTest) {
    EXPECT_FLOAT_EQ(3.7416575, v1.length());
    EXPECT_FLOAT_EQ(7.0710678, v2.length());
}

TEST_F(Vector3DTest, normalizeTest) {
    v1.normalize();
    EXPECT_FLOAT_EQ(0.26726124, v1.getX());
    EXPECT_FLOAT_EQ(0.53452247, v1.getY());
    EXPECT_FLOAT_EQ(0.80178368, v1.getZ());
}

TEST_F(Vector3DTest, dotTest) {
    EXPECT_FLOAT_EQ(26, v1.dot(v2));
}

TEST_F(Vector3DTest, crossTest) {
    Vector3D v3 = v1.cross(v2);
    EXPECT_FLOAT_EQ(-2, v3.getX());
    EXPECT_FLOAT_EQ(4, v3.getY());
    EXPECT_FLOAT_EQ(-2, v3.getZ());
}

TEST_F(Vector3DTest, lengthSquaredTest) {
    EXPECT_FLOAT_EQ(14, v1.lengthSquared());
    EXPECT_FLOAT_EQ(50, v2.lengthSquared());
}

TEST_F(Vector3DTest, opIndexTest) {
    EXPECT_FLOAT_EQ(1, v1[0]);
    EXPECT_FLOAT_EQ(2, v1[1]);
    EXPECT_FLOAT_EQ(3, v1[2]);
    EXPECT_ANY_THROW(v1[3]);
}

TEST_F(Vector3DTest, opVplusVTest) {
    Vector3D v3 = v1 + v2;
    EXPECT_FLOAT_EQ(4, v3.getX());
    EXPECT_FLOAT_EQ(6, v3.getY());
    EXPECT_FLOAT_EQ(8, v3.getZ());
}

TEST_F(Vector3DTest, opVminusVTest) {
    Vector3D v3 = v1 - v2;
    EXPECT_FLOAT_EQ(-2, v3.getX());
    EXPECT_FLOAT_EQ(-2, v3.getY());
    EXPECT_FLOAT_EQ(-2, v3.getZ());
}

TEST_F(Vector3DTest, opVmultVTest) {
    Vector3D v3 = v1 * v2;
    EXPECT_FLOAT_EQ(3, v3.getX());
    EXPECT_FLOAT_EQ(8, v3.getY());
    EXPECT_FLOAT_EQ(15, v3.getZ());
}

TEST_F(Vector3DTest, opCmultVTest) {
    float c = 5;
    Vector3D v3 = c * v1;
    EXPECT_FLOAT_EQ(5, v3.getX());
    EXPECT_FLOAT_EQ(10, v3.getY());
    EXPECT_FLOAT_EQ(15, v3.getZ());
}

TEST_F(Vector3DTest, opVmultCTest) {
    float c = 5;
    Vector3D v3 = v1 * c;
    EXPECT_FLOAT_EQ(5, v3.getX());
    EXPECT_FLOAT_EQ(10, v3.getY());
    EXPECT_FLOAT_EQ(15, v3.getZ());
}

TEST_F(Vector3DTest, opVdivVTest) {
    Vector3D v3 = v1 / v2;
    EXPECT_FLOAT_EQ(.33333334, v3.getX());
    EXPECT_FLOAT_EQ(.5, v3.getY());
    EXPECT_FLOAT_EQ(.6, v3.getZ());
}

TEST_F(Vector3DTest, opVdivCTest) {
    float c = 5;
    Vector3D v3 = v1 / c;
    EXPECT_FLOAT_EQ(0.2, v3.getX());
    EXPECT_FLOAT_EQ(0.4, v3.getY());
    EXPECT_FLOAT_EQ(0.6, v3.getZ());
}