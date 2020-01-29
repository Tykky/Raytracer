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
TEST_F(vector3Dtest, opAddtTestVector) {
    v1 += v2;
    EXPECT_FLOAT_EQ(4,v1.getX());
    EXPECT_FLOAT_EQ(6,v1.getY());
    EXPECT_FLOAT_EQ(8,v1.getZ());
}


/** Tests operator += constant */
TEST_F(vector3Dtest, opAddTestConstant) {
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
TEST_F(vector3Dtest, opDecreaseTestVector) {
    v1 -= v2;
    EXPECT_FLOAT_EQ(-2,v1.getX());
    EXPECT_FLOAT_EQ(-2, v1.getY());
    EXPECT_FLOAT_EQ(-2,v1.getZ());
}


/** Tests operator -= constant */
TEST_F(vector3Dtest,opDecreaseTestConstant) {
    float c = 5;
    v1 -= c;
    EXPECT_FLOAT_EQ(-4, v1.getX());
    EXPECT_FLOAT_EQ(-3, v1.getY());
    EXPECT_FLOAT_EQ(-2, v1.getZ());
}


/** Tests operator *= vector */
TEST_F(vector3Dtest, opMultiplyTestVector) {
    v1 *= v2;
    EXPECT_FLOAT_EQ(3, v1.getX());
    EXPECT_FLOAT_EQ(8, v1.getY());
    EXPECT_FLOAT_EQ(15, v1.getZ());
}

/** Tests operator *= constant */
TEST_F(vector3Dtest, opMultiplyTestConstant) {
    float c = 5;
    v1 *= c;
    EXPECT_FLOAT_EQ(5, v1.getX());
    EXPECT_FLOAT_EQ(10,v1.getY());
    EXPECT_FLOAT_EQ(15,v1.getZ());
}

/** Test operator /= vector */
TEST_F(vector3Dtest, opDivideTestVector) {
    v1 /= v2;
    EXPECT_FLOAT_EQ(.3333333, v1.getX());
    EXPECT_FLOAT_EQ(.5, v1.getY());
    EXPECT_FLOAT_EQ(.6, v1.getZ());
}

/** Test operator /= constant */
TEST_F(vector3Dtest, opDivideTestConstant) {
    float c = 5;
    v1 /=  c;
    EXPECT_FLOAT_EQ(0.2, v1.getX());
    EXPECT_FLOAT_EQ(0.4, v1.getY());
    EXPECT_FLOAT_EQ(0.6, v1.getZ());
}
