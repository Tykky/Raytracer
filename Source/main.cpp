#include <iostream>
#include <memory>
#include <fstream>
#include "vector3D.h"
#include "ray.h"
#include "camera.h"
#include "utility.h"
#include "primitives/sphere.h"
#include "primitives/primitivelist.h"
#include "cmath"
#include <chrono>

using namespace std;

int main() {

    const int width = 2560;
    const int height = 1440;

    const camera cam(100,float(width)/float(height));

    vector3D** framebuffer = new vector3D*[height];
    for (int i = 0; i < height; ++i) {
        framebuffer[i] = new vector3D[width];
    }

    primitive *list[4];
    list[0] = new sphere(vector3D(0,0,-7),1);
    list[1] = new sphere(vector3D(4,0,-7), 1);
    list[2] = new sphere(vector3D(-4,0,-7),1);
    list[3] = new sphere(vector3D(0,-10000,-7),9999);
    primitive *world = new primitivelist(list,4);

    auto start = chrono::system_clock::now();

    cout << "rendering started.." << endl;

#pragma omp parallel
    {
        #pragma omp for
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                ray r = cam.getRay(float(x) / float(width), float(y) / float(height));
                hitrecord rec;
                vector3D col;
                if (world->hit(r, 0, MAXFLOAT, rec)) {
                        col = sphereNormalColor(rec);
                } else {
                    col = skyGradient(r);
                }
                framebuffer[y][x] = col;
            }
        }
    }
    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end-start);

    cout << "rendering finished in " << double(elapsed.count())/1000 << " seconds" << endl;

    start = chrono::system_clock::now();
    cout << "writing to file... " << endl;
    ofstream of;
    of.open("image.ppm");
    of << "P3\n" << width << " " << height << "\n255\n";
    for (int y = height-1; y >=0 ; --y) {
        for (int x = 0; x < width; ++x) {
            of << int(framebuffer[y][x].getR()*255.99) << " "
               << int(framebuffer[y][x].getG()*255.99) << " "
               << int(framebuffer[y][x].getB()*255.99) << "\n";
        }
    }
    of.close();
    end = chrono::system_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end-start);
    cout << "writing finished in " << double(elapsed.count())/1000 << " seconds" << endl;

    return 0;
}