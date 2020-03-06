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

    int width = 800;
    int height = 600;
    int samples = 100;
    int fov = 90;
    int scene = 0;

    cout << "Enter image width: ";
    cin >> width;
    cout << "Enter image height: ";
    cin >> height;
    cout << "Enter camera preset (0-3): ";
    cin >> scene;
    cout << "Enter fov: ";
    cin >> fov;
    cout << "Enter amount of  samples: ";;
    cin >> samples;

    Camera cam(fov, float(width) / float(height), Vector3D(-0.2, 0.5, 1), Vector3D(0, 0.5, -1),
               Vector3D(0, 0.5, 0));

    if(scene == 1) {
        cam = Camera(fov, float(width) / float(height), Vector3D(0, 5, 1), Vector3D(0, 0.5, -1),
                     Vector3D(0, 0.5, 0));
    }

    if(scene == 2) {
        cam = Camera(fov, float(width) / float(height), Vector3D(10, 5, 5), Vector3D(0, 0.5, -1),
                     Vector3D(0, 0.5, 0));
    }

    if(scene == 3) {
        cam = Camera(fov, float(width) / float(height), Vector3D(100, 30, 50), Vector3D(0, 0.5, -1),
                     Vector3D(0, 0.5, 0));
    }

    int n = 50;

    Primitive *list[n+5];
    Brdf mat(Vector3D(0.3,0.3,0.8),Vector3D(0.2,1,0.1),0.1, 0.1, 1, 1);
    Brdf lamb(Vector3D(0.3,0.3,0.3),Vector3D(0.1,0.1,0.1),0, 0, 0.1, 1);
    Lambertian red(Vector3D(1, 0.2, 0.2));
    Dielectric glass = Dielectric(1.5);
    Brdf close(Vector3D(0.0,0.3,0.3),Vector3D(1,1,1), 0.1, 0, 1,1);
    Brdf gold(Vector3D(0.85,0.64,0.12),Vector3D(0.85,0.64,0.12),0.9,0.1,0,1);

    Material *matptr = &mat;
    Material *lambptr = &lamb;
    Material *glassptr = &glass;
    Material *redptr = &red;
    Material *closeptr = &close;

    mt19937 gen(456);
    uniform_real_distribution<float> dist(0.0, 1.0);
    function<float()> randomFloat = bind(dist, gen);

    int multp = 5;


    for (int i = 0; i < n; ++i) {
        float randi = randomFloat();
        float radius = randomFloat();
        if(randi < 0.25) {
            list[i] = new Sphere(Vector3D(multp * randomFloat() - multp/2, 0.1, -multp * randomFloat()), 0.1, glassptr);
        } else if (randi < 0.5) {
            list[i] = new Sphere(Vector3D(multp * randomFloat() - multp/2, 0.1, -multp * randomFloat()), 0.1, redptr);
        } else {
            list[i] = new Sphere(Vector3D(multp * randomFloat() - multp/2, 0.1, -multp * randomFloat()), 0.1, matptr);
        }
    }
    list[n] = new Sphere(Vector3D(0,-1000,0),1000,lambptr);

    list[n+1] = new Sphere(Vector3D(0,0.5,-1),0.5, closeptr);
    list[n+2] = new Sphere(Vector3D(-10,4.7,-20),5, matptr);
    list[n+3] = new Sphere(Vector3D(-1.5,0.5,-3),0.5, closeptr);
    list[n+4] = new Sphere(Vector3D(-1.5,15,-3),10, lambptr);

    Primitive *world = new Primitivelist(list,n+5);

    Primitive *bvh = new Bvhnode(list,n+5,0,1,randomFloat);

    Engine engine(bvh, cam, width, height);

    auto start = chrono::system_clock::now();
    cout << "rendering started.." << endl;

    engine.render(samples);

    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "rendering finished in " << double(elapsed.count()) / 1000 << " seconds" << endl;

    start = chrono::system_clock::now();
    cout << "writing to a file... " << endl;
    engine.frammebufferToNetpbm("image");
    end = chrono::system_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "writing finished in " << double(elapsed.count()) / 1000 << " seconds" << endl;

    return 0;
}