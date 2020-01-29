#include "gtest/gtest.h"
#include "vector3D.h"
#include <random>

using namespace std;

class vector3Dtest : public::testing::Test {
protected:

    vector3D v1;
    vector3D v2;
    vector3D vr1;
    vector3D vr2;

    default_random_engine gen;
    uniform_real_distribution<float> dist;

    float x1, y1, z1;
    float x2, y2, z2;

    virtual void SetUp() {

        v1 = vector3D(1,2,3);
        v2 = vector3D(3,4,5);

        gen = default_random_engine();
        dist = uniform_real_distribution<float>(-1000000,1000000);
        auto roll = bind(dist,gen);

        x1 = roll(); y1 = roll(); z1 = roll();
        x2 = roll(); y2 = roll(); z2 = roll();

        vr1 = vector3D(x1, y1, z1);
        vr2 = vector3D(x2, y2, z2);
    }

};

/**Tests operator +() */
TEST_F(vector3Dtest, opPlusTestSmall) {
    +(v1);
    EXPECT_FLOAT_EQ(1,v1.getX());
    EXPECT_FLOAT_EQ(2, v1.getY());
    EXPECT_FLOAT_EQ(3,v1.getZ());
}
TEST_F(vector3Dtest, opPlusTestRandom) {
    +(vr1);
    EXPECT_FLOAT_EQ(x1,vr1.getX());
    EXPECT_FLOAT_EQ(y1,vr1.getY());
    EXPECT_FLOAT_EQ(z1, vr1.getZ());
}

/** Tests operator += vector */
TEST_F(vector3Dtest, opAddtTestSmallVector) {
    v1 += v2;
    EXPECT_FLOAT_EQ(4,v1.getX());
    EXPECT_FLOAT_EQ(6,v1.getY());
    EXPECT_FLOAT_EQ(8,v1.getZ());
}
TEST_F(vector3Dtest, opAddTestRandomVector) {
    vr1 += vr2;
    EXPECT_FLOAT_EQ(x1+x2, vr1.getX());
    EXPECT_FLOAT_EQ(y1+y2, vr1.getY());
    EXPECT_FLOAT_EQ(z1+z2, vr1.getZ());
}

/** Tests operator += constant */
TEST_F(vector3Dtest, opAddTestSmallConstant) {
    float c = 9;
    v1 += c;
    EXPECT_FLOAT_EQ(1+c,v1.getX());
    EXPECT_FLOAT_EQ(2+c,v1.getY());
    EXPECT_FLOAT_EQ(3+c,v1.getZ());
}
TEST_F(vector3Dtest, opAddTestRandomConstant) {
    float c = dist(gen);
    vr1 += c;
    EXPECT_FLOAT_EQ(x1+c,vr1.getX());
    EXPECT_FLOAT_EQ(y1+c,vr1.getY());
    EXPECT_FLOAT_EQ(z1+c,vr1.getZ());
}
/** Tests operator -() */
TEST_F(vector3Dtest, opMinusTestSmall) {
    vector3D negv1 = -v1;
    EXPECT_FLOAT_EQ(-1, negv1.getX());
    EXPECT_FLOAT_EQ(-2, negv1.getY());
    EXPECT_FLOAT_EQ(-3, negv1.getZ());
}
TEST_F(vector3Dtest, opMinusTestRandom) {
    vector3D negvr1 = -vr1;
    EXPECT_FLOAT_EQ(-x1, negvr1.getX());
    EXPECT_FLOAT_EQ(-y1, negvr1.getY());
    EXPECT_FLOAT_EQ(-z1, negvr1.getZ());
}

/** Tests operator -= vector */
TEST_F(vector3Dtest, opDecreaseTestSmallVector) {
    v1 -= v2;
    EXPECT_FLOAT_EQ(-2,v1.getX());
    EXPECT_FLOAT_EQ(-2, v1.getY());
    EXPECT_FLOAT_EQ(-2,v1.getZ());
}
TEST_F(vector3Dtest, opDecreaseTestRandomVector) {
    vr1 -= vr2;
    EXPECT_FLOAT_EQ(x1-x2,vr1.getX());
    EXPECT_FLOAT_EQ(y1-y2, vr1.getY());
    EXPECT_FLOAT_EQ(z1-z2,vr1.getZ());
}

/** Tests operator -= constant */
TEST_F(vector3Dtest,opDecreaseTestSmallConstant) {
    float c = 5;
    v1 -= c;
    EXPECT_FLOAT_EQ(-4, v1.getX());
    EXPECT_FLOAT_EQ(-3, v1.getY());
    EXPECT_FLOAT_EQ(-2, v1.getZ());
}
TEST_F(vector3Dtest,opDecreaseTestRandomConstant) {
    float c = dist(gen);
    vr1 -= c;
    EXPECT_FLOAT_EQ(x1-c, vr1.getX());
    EXPECT_FLOAT_EQ(y1-c,vr1.getY());
    EXPECT_FLOAT_EQ(z1-c, vr1.getZ());
}

/** Tests operator *= vector */
TEST_F(vector3Dtest, opMultiplyTestSmallVector) {
    v1 *= v2;
    EXPECT_FLOAT_EQ(3, v1.getX());
    EXPECT_FLOAT_EQ(8, v1.getY());
    EXPECT_FLOAT_EQ(15, v1.getZ());
}
TEST_F(vector3Dtest, opMultiplyTesteRandomVector) {
    vr1 *= vr2;
    EXPECT_FLOAT_EQ(x1*x2, vr1.getX());
    EXPECT_FLOAT_EQ(y1*y2, vr1.getY());
    EXPECT_FLOAT_EQ(z1*z2, vr1.getZ());
}

/** Tests operator *= constant */
TEST_F(vector3Dtest, opMultiplyTestSmallConstant) {
    float c = 5;
    v1 *= c;
    EXPECT_FLOAT_EQ(5, v1.getX());
    EXPECT_FLOAT_EQ(10,v1.getY());
    EXPECT_FLOAT_EQ(15,v1.getZ());
}
TEST_F(vector3Dtest, opMultiplyTestRandomConstant) {
    float c = dist(gen);
    vr1 *= c;
    EXPECT_FLOAT_EQ(x1*c, vr1.getX());
    EXPECT_FLOAT_EQ(y1*c,vr1.getY());
    EXPECT_FLOAT_EQ(z1*c,vr1.getZ());
}

/** Test operator /= vector */
TEST_F(vector3Dtest, opDivideTestSmallVector) {
    v1 /= v2;
    EXPECT_FLOAT_EQ(.33333334, v1.getX());
    EXPECT_FLOAT_EQ(.5, v1.getY());
    EXPECT_FLOAT_EQ(.60000002, v1.getZ());
}
TEST_F(vector3Dtest, opDivideTestRandomVector) {
    vr1 /= vr2;
    EXPECT_FLOAT_EQ(x1/x2, vr1.getX());
    EXPECT_FLOAT_EQ(y1/y2, vr1.getY());
    EXPECT_FLOAT_EQ(z1/z2, vr1.getZ());
}

/** Test operator /= constant */
TEST_F(vector3Dtest, opDivideTestSmallConstant) {
    float c = 5;
    v1 /=  c;
    EXPECT_FLOAT_EQ(0.2, v1.getX());
    EXPECT_FLOAT_EQ(0.4, v1.getY());
    EXPECT_FLOAT_EQ(0.6, v1.getZ());
}
TEST_F(vector3Dtest, opDivideTestRandomConstant) {
    float c = dist(gen);
    vr1 /= c;
    EXPECT_FLOAT_EQ(x1/c, vr1.getX());
    EXPECT_FLOAT_EQ(y1/c, vr1.getY());
    EXPECT_FLOAT_EQ(z1/c, vr1.getZ());
}
