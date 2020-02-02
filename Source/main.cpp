#include <iostream>
#include <memory>
#include <fstream>
#include "vector3D.h"
#include "ray.h"
#include "camera.h"
#include "utility.h"


using namespace std;

int main() {

    int width = 1920;
    int height = 1080;

    ofstream of;
    of.open("image.ppm");
    of << "P3\n" << width << " " << height << "\n255\n";

    camera cam(90,float(width)/float(height));

    for (int y = height-1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            ray r = cam.getRay(float(x)/width,float(y)/height);
            vector3D col = skyGradient(r);
            of << int(255.99*col.getR()) << " " << int(255.99*col.getG()) << " " << int(255.99*col.getB()) << "\n";
        }
    }

    of.close();

    return 0;
}