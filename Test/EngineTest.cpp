#include "gtest/gtest.h"
#include "Engine.h"
#include "Camera.h"
#include "primitives/Sphere.h"
#include "materials/Brdf.h"
#include "primitives/Bvhnode.h"
#include <random>

class EngineTest : public ::testing::Test {
protected:

    Primitive *bvh;
    Engine *engine;
    Camera *camera;
    Primitive *list[2];
    Material *mat;
    int h, w;

    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
    function<float()> randomFloat;

    virtual void SetUp() {
        w = 800;
        h = 600;
        mat = new Brdf(Vector3D(0.5,0.5,0.5),Vector3D(1,1,1),0,0,1,1.3);
        camera = new Camera(90,800/600,Vector3D(0,0,0),Vector3D(0,0,-1),Vector3D(0,1,0));
        list[0] = new Sphere(Vector3D(0,0,-1),0.5,mat);
        list[1] = new Sphere(Vector3D(0,-100,-1),100,mat);

        gen = std::mt19937(1337);
        dist = std::uniform_real_distribution<float>(0,1);
        randomFloat = bind(dist,gen);

        bvh = new Bvhnode(list,2,0,1,randomFloat);

        engine = new Engine(bvh,*camera,w,h);
    }

    virtual void TearDown() {
        delete bvh;
        for (int i = 0; i < 2; ++i) {
            delete list[i];
        }
        delete mat;
    }

};

TEST_F(EngineTest, rendertest1) {
    engine->render(100);
}