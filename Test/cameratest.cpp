#include "gtest/gtest.h"
#include "camera.h"

using namespace std;

class cameratest : public::testing::Test {
protected:

    camera *cam;
    float halfw;
    float halfh;

    virtual void SetUp() {
        cam = new camera(90,float(16)/float(9));
        halfw = 1; // w = tan(fov/2)
        halfh = 0.5625; // h = halfw/(aspect ratio)
    }

    virtual void TearDown() {
        delete cam;
    }

};

TEST_F(cameratest, getEyetest) {
    ray r = cam->getRay(0,0);
    EXPECT_FLOAT_EQ(0,r.getA().getX());
    EXPECT_FLOAT_EQ(0,r.getA().getY());
    EXPECT_FLOAT_EQ(0,r.getA().getZ());
}
TEST_F(cameratest, getScreenCentertest) {
    ray r = cam->getRay(0.5,0.5);
    EXPECT_FLOAT_EQ(0,r.getB().getX());
    EXPECT_FLOAT_EQ(0,r.getB().getY());
    EXPECT_FLOAT_EQ(-1,r.getB().getZ());
}
TEST_F(cameratest, getScreenLowerLefttest) {
    ray r = cam->getRay(0,0);
    EXPECT_FLOAT_EQ(-halfw, r.getB().getX());
    EXPECT_FLOAT_EQ(-halfh,r.getB().getY());
    EXPECT_FLOAT_EQ(-1,r.getB().getZ());
}
TEST_F(cameratest, getScreenLowerRightttest) {
    ray r = cam->getRay(1,0);
    EXPECT_FLOAT_EQ(halfw, r.getB().getX());
    EXPECT_FLOAT_EQ(-halfh,r.getB().getY());
    EXPECT_FLOAT_EQ(-1,r.getB().getZ());
}
TEST_F(cameratest, getScreenTopLefttest) {
    ray r = cam->getRay(0,1);
    EXPECT_FLOAT_EQ(-halfw, r.getB().getX());
    EXPECT_FLOAT_EQ(halfh,r.getB().getY());
    EXPECT_FLOAT_EQ(-1,r.getB().getZ());
}
TEST_F(cameratest, getScreenTopRighttest) {
    ray r = cam->getRay(1,1);
    EXPECT_FLOAT_EQ(halfw, r.getB().getX());
    EXPECT_FLOAT_EQ(halfh,r.getB().getY());
    EXPECT_FLOAT_EQ(-1,r.getB().getZ());
}