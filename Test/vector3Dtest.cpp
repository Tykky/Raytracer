#include "gtest/gtest.h"
#include "vector3D.h"

class vector3DSmallTest : public::testing::Test {
protected:

    vector3D *v1;
    vector3D *v2;

    virtual void SetUp() {
        v1 = new vector3D(1,2,3);
        v2 = new vector3D(3,4,5);
    }
    virtual void TearDown() {
        delete v1;
        delete v2;
    }
};
TEST_F(vector3DSmallTest, opPlusTest1) {
    +(*v1);
    ASSERT_EQ(1,v1->getX());
    ASSERT_EQ(2, v1->getY());
    ASSERT_EQ(3,v1->getZ());
}

TEST_F(vector3DSmallTest, opPlustTest2) {
    (*v1) += (*v2);
    ASSERT_EQ(4,v1->getX());
    ASSERT_EQ(6,v1->getY());
    ASSERT_EQ(8,v1->getZ());
}