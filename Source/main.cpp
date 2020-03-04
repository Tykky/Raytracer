#include <iostream>
#include "Vector3D.h"
#include "Camera.h"
#include "Utility.h"
#include "primitives/Sphere.h"
#include "primitives/Primitivelist.h"
#include <chrono>
#include "materials/Lambertian.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"
#include "materials/Brdf.h"
#include "Engine.h"
#include "primitives/Bvhnode.h"

using namespace std;

int main(int argc, char **argv) {

    int width = 200;
    int height = 200;
    int samples = 100;
    int fov = 60;

    /*
    cout << "Enter screen width: ";
    cin >> width;
    cout << "Enter screen height: ";
    cin >> height;
    cout << "Enter fov: ";
    cin >> fov;
    cout << "Enter amount of samples: ";;
    cin >> samples;
    */

    const Camera cam(fov, float(width) / float(height), Vector3D(0, 3, 1), Vector3D(0, 3, -1),
                     Vector3D(0, 1, 0));


    int n = 100000;

    Primitive *list[n+1];
    Brdf mat(Vector3D(0.3,0.3,0.8),Vector3D(0.2,1,0.1),0.1, 0.1, 1, 1);
    Lambertian lamb(Vector3D(0.6, 0.6, 0.6));
    Lambertian red(Vector3D(1, 0.2, 0.2));
    Dielectric glass = Dielectric(1);
    Material *matptr = &mat;
    Material *lambptr = &lamb;
    Material *glassptr = &glass;
    Material *redptr = &red;

    mt19937 gen;
    uniform_real_distribution<float> dist(0.0, 1.0);
    function<float()> randomFloat = bind(dist, gen);

    float randi = randomFloat();

    for (int i = 0; i < n; ++i) {
        float radius = randomFloat();
        list[i] = new Sphere(Vector3D(50*randomFloat()-25,0.5,-50*randomFloat()),0.5,matptr);
    }

    list[n] = new Sphere(Vector3D(0,-1000,0),1000,lambptr);

    Primitive *world = new Primitivelist(list,n+1);

    Primitive *bvh = new Bvhnode(list,n+1,0,1,randomFloat);

    Engine engine(bvh, cam, width, height);

    auto start = chrono::system_clock::now();
    cout << "rendering started.." << endl;

    engine.render(samples);

    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "rendering finished in " << double(elapsed.count()) / 1000 << " seconds" << endl;

    start = chrono::system_clock::now();
    cout << "writing to file... " << endl;
    engine.frammebufferToNetpbm("image");
    end = chrono::system_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "writing finished in " << double(elapsed.count()) / 1000 << " seconds" << endl;

    return 0;
}