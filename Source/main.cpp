#include <iostream>
#include <memory>
#include <fstream>
#include "vector3D.h"
#include "ray.h"
#include "camera.h"
#include "utility.h"
#include "primitives/sphere.h"

using namespace std;

int main() {

    int width = 2560;
    int height = 1440;

    ofstream of;
    of.open("image.ppm");
    of << "P3\n" << width << " " << height << "\n255\n";

    camera cam(100,float(width)/float(height));

    for (int y = height-1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            ray r = cam.getRay(float(x)/float(width),float(y)/float(height));
            sphere sp1(vector3D(0,0,-7),1);
            sphere sp2(vector3D(4,0,-7), 1);
            sphere sp3(vector3D(-4,0,-7),1);
            sphere sp4(vector3D(0,-100,-7),100);
            hitrecord record1;
            hitrecord record2;
            hitrecord record3;
            hitrecord record4;
            vector3D col = vector3D();
            if(sp1.hit(r,0, 10000000.0,record1)) {
                col = sphereNormalColor(record1);
            } else if(sp2.hit(r,0,10000000.0,record2)) {
                col = sphereNormalColor(record2);
            } else if(sp3.hit(r,0,10000000.0,record3)) {
                col = sphereNormalColor(record3);
            } else if(sp4.hit(r,0,10000000.0,record4)) {
                col = sphereNormalColor(record4);
            } else {
                col = skyGradient(r);
            }
            of << int(255.99*col.getR()) << " " << int(255.99*col.getG()) << " " << int(255.99*col.getB()) << "\n";
        }
    }

    of.close();

    return 0;
}