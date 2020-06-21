#include "gtest/gtest.h"
#include "core/Camera.h"

/** @brief Googletest fixture for testing Camera class */
class Cameratest : public ::testing::Test {
protected:

    Camera *cam;
    float halfw;
    float halfh;

    virtual void SetUp() {
        cam = new Camera(90, float(16) / float(9), Vector3D(0, 0, 0), Vector3D(0, 0, -1),
                         Vector3D(0, 1, 0));
        halfw = 1; // w = tan(fov/2)
        halfh = 0.5625; // h = halfw/(aspect ratio)
    }

    virtual void TearDown() {
        delete cam;
    }

};

TEST_F(Cameratest, getEyetest) {
    Ray r = cam->getRay(0, 0);
    EXPECT_FLOAT_EQ(0, r.getOrigin().getX());
    EXPECT_FLOAT_EQ(0, r.getOrigin().getY());
    EXPECT_FLOAT_EQ(0, r.getOrigin().getZ());
}

TEST_F(Cameratest, getScreenCentertest) {
    Ray r = cam->getRay(0.5, 0.5);
    EXPECT_FLOAT_EQ(0, r.getDirection().getX());
    EXPECT_FLOAT_EQ(0, r.getDirection().getY());
    EXPECT_FLOAT_EQ(-1, r.getDirection().getZ());
}

TEST_F(Cameratest, getScreenLowerLefttest) {
    Ray r = cam->getRay(0, 0);
    EXPECT_FLOAT_EQ(-halfw, r.getDirection().getX());
    EXPECT_FLOAT_EQ(-halfh, r.getDirection().getY());
    EXPECT_FLOAT_EQ(-1, r.getDirection().getZ());
}

TEST_F(Cameratest, getScreenLowerRightttest) {
    Ray r = cam->getRay(1, 0);
    EXPECT_FLOAT_EQ(halfw, r.getDirection().getX());
    EXPECT_FLOAT_EQ(-halfh, r.getDirection().getY());
    EXPECT_FLOAT_EQ(-1, r.getDirection().getZ());
}

TEST_F(Cameratest, getScreenTopLefttest) {
    Ray r = cam->getRay(0, 1);
    EXPECT_FLOAT_EQ(-halfw, r.getDirection().getX());
    EXPECT_FLOAT_EQ(halfh, r.getDirection().getY());
    EXPECT_FLOAT_EQ(-1, r.getDirection().getZ());
}

TEST_F(Cameratest, getScreenTopRighttest) {
    Ray r = cam->getRay(1, 1);
    EXPECT_FLOAT_EQ(halfw, r.getDirection().getX());
    EXPECT_FLOAT_EQ(halfh, r.getDirection().getY());
    EXPECT_FLOAT_EQ(-1, r.getDirection().getZ());
}