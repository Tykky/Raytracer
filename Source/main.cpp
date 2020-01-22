#include <iostream>
#include <memory>
#include "Dummyclass.h"

using namespace std;

int main() {
    unique_ptr<Dummyclass> dummy(new Dummyclass(10,15));
    cout << "t1: " << dummy->getT1() << endl;
    cout << "t2: " << dummy->getT2() << endl;
    cout << "dummymethod: " << dummy->dummymethod2(2) << endl;

    return 0;
}