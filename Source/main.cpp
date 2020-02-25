#include <iostream>
#include "vector3D.h"
#include "Camera.h"
#include "Utility.h"
#include "primitives/Sphere.h"
#include "primitives/Primitivelist.h"
#include <chrono>
#include "materials/Lambertian.h"
#include "materials/Metal.h"
#include "materials/Dielectric.h"
#include "Engine.h"

using namespace std;

int main(int argc, char **argv) {

    int width = 800;
    int height = 600;
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

    const Camera cam(fov, float(width) / float(height), vector3D(0,0,0),vector3D(0,0,-1),
                 vector3D(0,1,0));

    Primitive *list[100];
    Metal mat(vector3D(0.8, 0.6, 0.2), 0.2);
    Lambertian lamb(vector3D(0.5, 0.5, 0.5));
    Lambertian red(vector3D(1, 0.2, 0.2));
    Dielectric glass = Dielectric(1.5);
    Material *matptr = &mat;
    Material *lambptr = &lamb;
    Material *glassptr = &glass;
    Material *redptr = &red;

    list[0] = new Sphere(vector3D(0, 0, -2), 0.5, glassptr);
    list[12] = new Sphere(vector3D(0, 0, -2), 0.49, lambptr);
    list[1] = new Sphere(vector3D(1, 0, -2), 0.5, matptr);
    list[2] = new Sphere(vector3D(-1, 0, -2), 0.5, matptr);
    list[3] = new Sphere(vector3D(0, -100.5, 0), 100, lambptr);
    list[4] = new Sphere(vector3D(0, -0.4, -1.5), 0.1, matptr);
    list[5] = new Sphere(vector3D(0.4, -0.4, -1.5), 0.1, glassptr);
    list[6] = new Sphere(vector3D(0.8, -0.415, -1.5), 0.1, matptr);
    list[7] = new Sphere(vector3D(1.2, -0.415, -1.5), 0.1, matptr);
    list[8] = new Sphere(vector3D(-0.4, -0.4, -1.5), 0.1, glassptr);
    list[9] = new Sphere(vector3D(-0.8, -0.41, -1.5), 0.1, matptr);
    list[10] = new Sphere(vector3D(-1.2, -0.415, -1.5), 0.1, matptr);
    list[11] = new Sphere(vector3D(0, 105, -4), 90, lambptr);
    list[12] = new Sphere(vector3D(0, 0, -2), 0.48, redptr);
    Primitive *world = new Primitivelist(list, 13);

    Engine engine(world, cam, width, height);

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