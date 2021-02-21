#include "gtest/gtest.h"
#include "core/Vector3D.h"
#include "hittables/Triangle.h"
#include "materials/Lambertian.h"
#include <memory>

class TriangleTest : public ::testing::Test {
protected:
    std::unique_ptr<Material> mat;
    std::unique_ptr<Triangle> tri;

    virtual void SetUp() {
        mat = std::make_unique<Lambertian>(Vector3D(1,1,1));
        tri = std::make_unique<Triangle>(Vector3D(0,0,-1), Vector3D(1,1,0), Vector3D(0,0,1), Vector3D(-1,0,0), mat.get());
    }
};

TEST_F(TriangleTest, hitCenterTest) {
    Hitrecord record;
    Ray r({-2,0.5,0}, {1,0,0});
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_FLOAT_EQ(record.distance, 2.5f);
    EXPECT_FLOAT_EQ(record.p.getX(), 0.5f);
    EXPECT_FLOAT_EQ(record.p.getY(), 0.5f);
    EXPECT_FLOAT_EQ(record.p.getZ(), 0.f);
    EXPECT_FLOAT_EQ(record.normal.getX(), -1.f);
    EXPECT_FLOAT_EQ(record.normal.getY(), 0.f);
    EXPECT_FLOAT_EQ(record.normal.getZ(), 0.f);
    EXPECT_EQ(record.matptr, mat.get());
}

TEST_F(TriangleTest, missUpperLeftTest) {
    Hitrecord record;
    Ray r({-2,0.8,-0.8}, {1,0,0});
    EXPECT_FALSE(tri->hit(r, 0, 99, record));
}

TEST_F(TriangleTest, hitLeftVertexTest) {
    Hitrecord record;
    Ray r({-2,0,-1}, {1,0,0});
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_FLOAT_EQ(record.distance, 2.f);
    EXPECT_FLOAT_EQ(record.p.getX(), 0.f);
    EXPECT_FLOAT_EQ(record.p.getY(), 0.f);
    EXPECT_FLOAT_EQ(record.p.getZ(), -1.f);
    EXPECT_FLOAT_EQ(record.normal.getX(), -1.f);
    EXPECT_FLOAT_EQ(record.normal.getY(), 0.f);
    EXPECT_FLOAT_EQ(record.normal.getZ(), 0.f);
    EXPECT_EQ(record.matptr, mat.get());
}

TEST_F(TriangleTest, hitRightVertexTest) {
    Hitrecord record;
    Ray r({-2,0,1}, {1,0,0});
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_FLOAT_EQ(record.distance, 2.f);
    EXPECT_FLOAT_EQ(record.p.getX(), 0.f);
    EXPECT_FLOAT_EQ(record.p.getY(), 0.f);
    EXPECT_FLOAT_EQ(record.p.getZ(), 1.f);
    EXPECT_FLOAT_EQ(record.normal.getX(), -1.f);
    EXPECT_FLOAT_EQ(record.normal.getY(), 0.f);
    EXPECT_FLOAT_EQ(record.normal.getZ(), 0.f);
    EXPECT_EQ(record.matptr, mat.get());
}

TEST_F(TriangleTest, hitTopeVertexTest) {
    Hitrecord record;
    Ray r({-2,1,0}, {1,0,0});
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_TRUE(tri->hit(r, 0, 99, record));
    EXPECT_FLOAT_EQ(record.distance, 3.f);
    EXPECT_FLOAT_EQ(record.p.getX(), 1.f);
    EXPECT_FLOAT_EQ(record.p.getY(), 1.f);
    EXPECT_FLOAT_EQ(record.p.getZ(), 0.f);
    EXPECT_FLOAT_EQ(record.normal.getX(), -1.f);
    EXPECT_FLOAT_EQ(record.normal.getY(), 0.f);
    EXPECT_FLOAT_EQ(record.normal.getZ(), 0.f);
    EXPECT_EQ(record.matptr, mat.get());
}

TEST_F(TriangleTest, plusVectorOperatorTest) {
    Vector3D vec(1.f,1.f,1.f);
    (*tri) += vec;
    EXPECT_EQ(tri->getVertex(0).getX(), 1.f);
    EXPECT_EQ(tri->getVertex(0).getY(), 1.f);
    EXPECT_EQ(tri->getVertex(0).getZ(), 0.f);
    EXPECT_EQ(tri->getVertex(1).getX(), 2.f);
    EXPECT_EQ(tri->getVertex(1).getY(), 2.f);
    EXPECT_EQ(tri->getVertex(1).getZ(), 1.f);
    EXPECT_EQ(tri->getVertex(2).getX(), 1.f);
    EXPECT_EQ(tri->getVertex(2).getY(), 1.f);
    EXPECT_EQ(tri->getVertex(2).getZ(), 2.f);
}

TEST_F(TriangleTest, surroundingBoxTest) {
    const float DELTA = 0.01f;
    Aabb box;
    EXPECT_TRUE(tri->boundingBox(0, 0, box));
    EXPECT_FLOAT_EQ(box.getMin().getX(), -DELTA);
    EXPECT_FLOAT_EQ(box.getMin().getY(), -DELTA);
    EXPECT_FLOAT_EQ(box.getMin().getZ(), -1.f - DELTA);
    EXPECT_FLOAT_EQ(box.getMax().getX(), 1.0f + DELTA);
    EXPECT_FLOAT_EQ(box.getMax().getY(), 1.0f + DELTA);
    EXPECT_FLOAT_EQ(box.getMax().getZ(), 1.0f + DELTA);
}