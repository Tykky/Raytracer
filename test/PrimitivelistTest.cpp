#include <hittables/Sphere.h>
#include <materials/Lambertian.h>
#include "gtest/gtest.h"
#include "hittables/Hitlist.h"

class PrimitivelistTest : public ::testing::Test
        {
protected:

    std::vector<std::shared_ptr<Hittable>> list;
    std::shared_ptr<Material> mat;
    std::shared_ptr<Hitlist> plist;

    void SetUp()
    {
        list = std::vector<std::shared_ptr<Hittable>>(3);
        mat = std::make_shared<Lambertian>(Vector3D(1,1,1));
        list[0] = std::make_unique<Sphere>(Vector3D(0,0,0),0.5,mat.get());
        list[1] = std::make_unique<Sphere>(Vector3D(-1,0,0),0.5,mat.get());
        list[2] = std::make_unique<Sphere>(Vector3D(1,0,0),0.5,mat.get());
        plist = std::make_shared<Hitlist>(list);
    }
};

TEST_F(PrimitivelistTest, hitMiddleTest)
{
    Ray r(Vector3D(0,0,-2),Vector3D(0,0,-1));
    Hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true, hit);
    EXPECT_FLOAT_EQ(0.5, record.p.getZ());

}

TEST_F(PrimitivelistTest, hitLeftTest)
{
    Ray r(Vector3D(-1,0,-2),Vector3D(-1,0,-1));
    Hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(PrimitivelistTest, hitLeftRight)
{
    Ray r(Vector3D(1,0,-2),Vector3D(1,0,-1));
    Hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);

    EXPECT_FLOAT_EQ(true,hit);
    EXPECT_FLOAT_EQ(0.3535533,record.p.getZ());
}

TEST_F(PrimitivelistTest, missTest)
{
    Ray r(Vector3D(5,0,-2),Vector3D(5,0,-1));
    Hitrecord record;

    bool hit = plist->hit(r,-9999,99999,record);
    EXPECT_EQ(false,hit);
    EXPECT_EQ(0,record.p.getZ());
}