#include "gtest/gtest.h"
#include "materials/lambertian.h"
#include <random>
#include <functional>

using namespace std;

class lambertiantest : public::testing::Test {
protected:

    material *mat;
    mt19937 engine;
    uniform_real_distribution<float> dist;
    function<float()> randomfloat;

    virtual void SetUp() {
        mat = new lambertian(vector3D(0.5,0.5,0.5));
        engine = mt19937(1337);
        dist = uniform_real_distribution<float>(0,1);
        randomfloat = bind(dist,engine);
    }
};


TEST_F(lambertiantest, scattertest) {

    hitrecord record;
    record.normal = vector3D(-1,1,0);
    record.p = vector3D(1,1,0);
    record.c = 1;

    vector3D attenuation;
    ray scatter;
    EXPECT_EQ(true, mat->scatter(ray(),record,attenuation,scatter,randomfloat));

    EXPECT_EQ(attenuation.getX(),0.5);
    EXPECT_EQ(attenuation.getY(),0.5);
    EXPECT_EQ(attenuation.getZ(),0.5);

    vector3D scat = scatter.getB()-scatter.getA();
    EXPECT_LE((record.normal-scatter.getB()).length(),1);


}