#include <iostream>
#include <fstream>
#include "vector3D.h"
#include "ray.h"
#include "camera.h"
#include "utility.h"
#include "primitives/sphere.h"
#include "primitives/primitivelist.h"
#include <chrono>
#include <limits>
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"
#include <cmath>

using namespace std;

int main(int argc, char **argv) {

    int width = 800;
    int height = 400;
    int samples = 100;
    int fov = 90;


    cout << "Enter screen width: ";
    cin >> width;
    cout << "Enter screen height: ";
    cin >> height;
    cout << "Enter fov: ";
    cin >> fov;
    cout << "Enter amount of samples: ";;
    cin >> samples;


    const camera cam(fov,float(width)/float(height));

    vector3D** framebuffer = new vector3D*[height];
    for (int i = 0; i < height; ++i) {
        framebuffer[i] = new vector3D[width];
    }

    primitive *list[100];
    metal mat(vector3D(0.8,0.6,0.2),0.2);
    lambertian lamb(vector3D(0.5,0.5,0.5));
    lambertian red(vector3D(1,0.2,0.2));
    dielectric glass = dielectric(1.5);
    material *matptr = &mat;
    material *lambptr = &lamb;
    material *glassptr = &glass;
    material *redptr = &red;

    list[0] = new sphere(vector3D(0,0,-2),0.5,glassptr);
    list[12] = new sphere(vector3D(0,0,-2),0.49,lambptr);
    list[1] = new sphere(vector3D(1,0,-2), 0.5, matptr);
    list[2] = new sphere(vector3D(-1,0,-2),0.5, matptr);
    list[3] = new sphere(vector3D(0,-100.5,0),100, lambptr);
    list[4] = new sphere(vector3D(0,-0.4,-1.5),0.1, matptr);
    list[5] = new sphere(vector3D(0.4,-0.4,-1.5),0.1, glassptr);
    list[6] = new sphere(vector3D(0.8,-0.415,-1.5),0.1, matptr);
    list[7] = new sphere(vector3D(1.2,-0.415,-1.5),0.1, matptr);
    list[8] = new sphere(vector3D(-0.4,-0.4,-1.5),0.1, glassptr);
    list[9] = new sphere(vector3D(-0.8,-0.41,-1.5),0.1, matptr);
    list[10] = new sphere(vector3D(-1.2,-0.415,-1.5),0.1, matptr);
    list[11] = new sphere(vector3D(0,105,-4),90, lambptr);
    list[12] = new sphere(vector3D(0,0,-2),0.48,redptr);
    primitive *world = new primitivelist(list,13);

    auto start = chrono::system_clock::now();

    cout << "rendering started.." << endl;

#pragma omp parallel
    {
        mt19937 engine;
        uniform_real_distribution<float> dist(0.0,1.0);
        function<float()> randomFloat = bind(dist,engine);

        #pragma omp for
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                vector3D color = vector3D();
                for(int s = 0; s < samples; s++) {
                    ray r = cam.getRay((float(x)+randomFloat()) / float(width), (float(y)+randomFloat()) / float(height));
                    hitrecord rec;
                    if (world->hit(r, 0, numeric_limits<float>::max(), rec)) {
                        color += recursiveScatter(r, world, 0,randomFloat);
                    } else {
                        color += skyGradient(r);
                    }
                }
                color/=float(samples);
                color = vector3D(sqrt(color.getR()),sqrt(color.getG()),sqrt(color.getB()));
                framebuffer[y][x] = color;
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