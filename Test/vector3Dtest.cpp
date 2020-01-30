#include "gtest/gtest.h"
#include "vector3D.h"

using namespace std;

class vector3Dtest : public::testing::Test {
protected:

    vector3D v1;
    vector3D v2;

    virtual void SetUp() {
        v1 = vector3D(1,2,3);
        v2 = vector3D(3,4,5);
    }

};

/**Tests operator +() */
TEST_F(vector3Dtest, opPlusTest) {
    +(v1);
    EXPECT_FLOAT_EQ(1,v1.getX());
    EXPECT_FLOAT_EQ(2, v1.getY());
    EXPECT_FLOAT_EQ(3,v1.getZ());
}


/** Tests operator += vector */
TEST_F(vector3Dtest, opAddtVectorTest) {
    v1 += v2;
    EXPECT_FLOAT_EQ(4,v1.getX());
    EXPECT_FLOAT_EQ(6,v1.getY());
    EXPECT_FLOAT_EQ(8,v1.getZ());
}


/** Tests operator += constant */
TEST_F(vector3Dtest, opAddConstantTest) {
    float c = 9;
    v1 += c;
    EXPECT_FLOAT_EQ(1+c,v1.getX());
    EXPECT_FLOAT_EQ(2+c,v1.getY());
    EXPECT_FLOAT_EQ(3+c,v1.getZ());
}

/** Tests operator -() */
TEST_F(vector3Dtest, opMinusTest) {
    vector3D negv1 = -v1;
    EXPECT_FLOAT_EQ(-1, negv1.getX());
    EXPECT_FLOAT_EQ(-2, negv1.getY());
    EXPECT_FLOAT_EQ(-3, negv1.getZ());
}


/** Tests operator -= vector */
TEST_F(vector3Dtest, opDecreaseVectorTest) {
    v1 -= v2;
    EXPECT_FLOAT_EQ(-2,v1.getX());
    EXPECT_FLOAT_EQ(-2, v1.getY());
    EXPECT_FLOAT_EQ(-2,v1.getZ());
}


/** Tests operator -= constant */
TEST_F(vector3Dtest,opDecreaseConstantTest) {
    float c = 5;
    v1 -= c;
    EXPECT_FLOAT_EQ(-4, v1.getX());
    EXPECT_FLOAT_EQ(-3, v1.getY());
    EXPECT_FLOAT_EQ(-2, v1.getZ());
}


/** Tests operator *= vector */
TEST_F(vector3Dtest, opMultiplyVectorTest) {
    v1 *= v2;
    EXPECT_FLOAT_EQ(3, v1.getX());
    EXPECT_FLOAT_EQ(8, v1.getY());
    EXPECT_FLOAT_EQ(15, v1.getZ());
}

/** Tests operator *= constant */
TEST_F(vector3Dtest, opMultiplyConstantTest) {
    float c = 5;
    v1 *= c;
    EXPECT_FLOAT_EQ(5, v1.getX());
    EXPECT_FLOAT_EQ(10,v1.getY());
    EXPECT_FLOAT_EQ(15,v1.getZ());
}

/** Test operator /= vector */
TEST_F(vector3Dtest, opDivideVectorTest) {
    v1 /= v2;
    EXPECT_FLOAT_EQ(.3333333, v1.getX());
    EXPECT_FLOAT_EQ(.5, v1.getY());
    EXPECT_FLOAT_EQ(.6, v1.getZ());
}

/** Test operator /= constant */
TEST_F(vector3Dtest, opDivideConstantTest) {
    float c = 5;
    v1 /=  c;
    EXPECT_FLOAT_EQ(0.2, v1.getX());
    EXPECT_FLOAT_EQ(0.4, v1.getY());
    EXPECT_FLOAT_EQ(0.6, v1.getZ());
}

TEST_F(vector3Dtest, lengthTest) {
    EXPECT_FLOAT_EQ(3.7416575,v1.length());
    EXPECT_FLOAT_EQ(7.0710678, v2.length());
}

TEST_F(vector3Dtest, normalizeTest) {
    v1.normalize();
    EXPECT_FLOAT_EQ(0.26726124,v1.getX());
    EXPECT_FLOAT_EQ(0.53452247, v1.getY());
    EXPECT_FLOAT_EQ(0.80178368, v1.getZ());
}

TEST_F(vector3Dtest, dotTest) {
    EXPECT_FLOAT_EQ(26,v1.dot(v2));
}

TEST_F(vector3Dtest, crossTest) {
    vector3D v3 = v1.cross(v2);
    EXPECT_FLOAT_EQ(-2, v3.getX());
    EXPECT_FLOAT_EQ(4, v3.getY());
    EXPECT_FLOAT_EQ(-2, v3.getZ());
}

TEST_F(vector3Dtest, lengthSquaredTest) {
    EXPECT_FLOAT_EQ(14,v1.lengthSquared());
    EXPECT_FLOAT_EQ(50,v2.lengthSquared());
}

TEST_F(vector3Dtest, opIndexTest) {
    EXPECT_FLOAT_EQ(1,v1[0]);
    EXPECT_FLOAT_EQ(2,v1[1]);
    EXPECT_FLOAT_EQ(3,v1[2]);
    EXPECT_ANY_THROW(v1[2]);
}