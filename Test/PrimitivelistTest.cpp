#include <primitives/Sphere.h>
#include <materials/Lambertian.h>
#include "gtest/gtest.h"
#include "primitives/Primitivelist.h";


class PrimitiveListTest : public ::testing::Test {
protected:

    Primitive *list[3];
    Material *mat;
    Primitivelist *plist;

    void SetUp() {
        mat = new Lambertian(Vector3D(1,1,1));
        list[0] = new Sphere(Vector3D(0,0,0),0.5,mat);
        list[1] = new Sphere(Vector3D(-1,0,0),0.5,mat);
        list[2] = new Sphere(Vector3D(1,0,0),0.5,mat);
        plist = new Primitivelist(list,3);
    }

    void TearDown() {
        for (int i = 0; i < 3; ++i) {
            delete list[i];
        }
        delete plist;
    }
};

TEST_F(PrimitiveListTest, hitMiddleTest) {

    Ray r(Vector3D(0,0,-2),Vector3D(0,0,-1));
    hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true, hit);
    EXPECT_FLOAT_EQ(0.5, record.p.getZ());

}

TEST_F(PrimitiveListTest, hitLeftTest) {
    Ray r(Vector3D(-1,0,-2),Vector3D(-1,0,-1));
    hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(PrimitiveListTest, hitLeftRight) {
    Ray r(Vector3D(1,0,-2),Vector3D(1,0,-1));
    hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(PrimitiveListTest, missTest) {
    Ray r(Vector3D(5,0,-2),Vector3D(5,0,-1));
    hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);
    EXPECT_EQ(false,hit);
    EXPECT_EQ(0,record.p.getZ());
}