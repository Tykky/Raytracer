# include "Triangle.h"

Triangle::Triangle() : v1_({}), v2_({}), v3_({}) {
}

Triangle::Triangle(Vector3D &v1, Vector3D &v2, Vector3D &v3) :
    v_1(v1), v2_(v2), v3_(v3) {
}

Triangle::hit(const Ray &r, float dmin, float dmax, Hitrecord &record) {
    
}

Triangle::boundingBox(float c0, float c_1, Aabb &box) {

}