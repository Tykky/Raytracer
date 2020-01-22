#ifndef RAYTRACER_DUMMYCLASS_H
#define RAYTRACER_DUMMYCLASS_H


class Dummyclass {

    int t1_;
    int t2_;

public:

    Dummyclass(int t1, int t2);
    int dummymethod1(int a);
    int dummymethod2(int a);

    int getT1() const;
    int getT2() const;

private:

    int dummymethod3(int a);

};


#endif //RAYTRACER_DUMMYCLASS_H
