#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>

using namespace std;

int main() {
    int nx = 200;
    int ny = 100;

    ofstream of;
    auto t1 = chrono::high_resolution_clock::now();
    of.open("image.ppm");
    of << "P3\n" << nx << " " <<  ny << "\n255\n";
    for (int i = 0; i < ny; ++i) {
        for (int j = 0; j < nx; ++j) {
            float r = float(i)/float(ny);
            float g = float(j)/float(nx);
            float b = 0.2;
            of << int(r*256) << " " << int(g*256) << " " << int(b*256) << "    ";
        }
        of << "\n";
    }
    of.close();
    auto t2 = chrono::high_resolution_clock::now();
    float time = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(t2-t1).count())/1000;
    cout << time << " seconds" << endl;
    return 0;
}