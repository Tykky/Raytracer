#include "gtest/gtest.h"
#include "core/Engine.h"
#include "core/Camera.h"
#include "primitives/Sphere.h"
#include "materials/Brdf.h"
#include "primitives/Bvhnode.h"
#include <random>
#include <fstream>

/** @brief Googletest fixture for testing the Engine class. */

const int w = 200;
const int h = 200;

class EngineTest : public ::testing::Test {
protected:

    Primitive *bvh;
    Engine *engine;
    Camera *camera;
    Primitive *list[2];
    Material *mat;

    std::mt19937 gen;
    std::uniform_real_distribution<float> dist;
    std::function<float()> randomFloat;

    virtual void SetUp() {
        mat = new Brdf(Vector3D(0.5,0.5,0.5),Vector3D(1,1,1),0,0,1,1.3);
        camera = new Camera(90,800/600,Vector3D(0,0.5,0),Vector3D(0,0.5,-1),Vector3D(0,1,0));
        list[0] = new Sphere(Vector3D(0,0.5,-1),0.5,mat);
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
        delete engine;
        delete camera;
    }

};

TEST_F(EngineTest, pixelTest) {
    engine->render(10);
    engine->frammebufferToNetpbm("test");

    Vector3D pixarray[h][w];

    std::ifstream file("test.ppm");

    std::string line;
    int linect = 0;

    int y = 0;

    // Read .ppm and parse
    while(std::getline(file,line)) {
        if(linect > 2) {
            int i = 0;

            std::string red = "";
            while(line[i] != ' ') {
                red += line[i];
                i++;
            }
            i++;
            std::string green = "";
            while(line[i] != ' ') {
                green += line[i];
                i++;
            }
            i++;
            std::string blue = "";
            while(i < line.length()) {
                blue += line[i];
                i++;
            }

            // Test that all of the pixels are in correct range (0-255)

            ASSERT_GE(stoi(red),0);
            ASSERT_LE(stoi(red),255);

            ASSERT_GE(stoi(green), 0);
            ASSERT_LE(stoi(green), 255);

            ASSERT_GE(stoi(blue), 0);
            ASSERT_LE(stoi(blue),255);

            int pixelnum = linect -3;

            if(pixelnum != 0 && pixelnum % w == 0) {
                y++;
            }

            int x = pixelnum % w;

            // translate pixels back to range (0-1) and write to array
            pixarray[y][x] = Vector3D(float(stoi(red))/float(255),float(stoi(green))/float(255),float(stoi(blue))/float(255));

        }
        linect++;
    }

    // Center of sphere should be gray
    EXPECT_NEAR(pixarray[h/2][w/2].getR(),0.5,0.15);
    EXPECT_NEAR(pixarray[h/2][w/2].getG(),0.5,0.15);
    EXPECT_NEAR(pixarray[h/2][w/2].getB(),0.5,0.15);

    // Sky should be blue
    EXPECT_NEAR(pixarray[h-1][w/2].getR(),0.45,0.15);
    EXPECT_NEAR(pixarray[h-1][w/2].getG(),0.45,0.15);
    EXPECT_NEAR(pixarray[h-1][w/2].getB(), 0.7,0.15);


}