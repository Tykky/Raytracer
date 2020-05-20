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
#include <QApplication>
#include <QPushButton>

using namespace std;

int main(int argc, char **argv) {

    QApplication a( argc, argv );
    QPushButton button( "Hello world!" );
    button.show();
    return a.exec();

}