#include "Dummyclass.h"


Dummyclass::Dummyclass(int t1, int t2) {
    t1_ = t1;
    t2_ = t2;
}

// NOT tested codecov should detect
int Dummyclass::dummymethod1(int a) {
    return a+1;
}

int Dummyclass::dummymethod2(int a) {
    return a+2+this->dummymethod3(a);
}

int Dummyclass::dummymethod3(int a) {
    return a+3;
}

int Dummyclass::getT1() const {
    return t1_;
}

int Dummyclass::getT2() const {
    return t2_;
}

