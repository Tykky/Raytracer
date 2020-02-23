
#ifndef RAYTRACER_VECTOR3D_H
#define RAYTRACER_VECTOR3D_H

#include <iostream>
#include <cmath>

/**@brief vector3D class containing data structure for 3D-euclidean and RGB vectors. **/
class vector3D {

private:
    float x, y, z;

public:
    /** @brief Initializes everything to 0. */
    vector3D() :
            x(0), y(0), z(0) {
    }

    /** @brief Initializes to parameter values. */
    vector3D(float x, float y, float z) :
            x(x), y(y), z(z) {
    }

    /**@brief Computes vector length with pythagoras sqrt(x^2+y^2+z^2) */
    inline float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    /**@brief Transforms vector to length 1 vector. */
    inline void normalize() {
        float k = 1 / std::sqrt(x * x + y * y + z * z);
        x *= k;
        y *= k;
        z *= k;
    }

    inline float dot(const vector3D &v) const {
        return x * v.getX() +
               y * v.getY() +
               z * v.getZ();
    }

    inline vector3D cross(const vector3D &v) const {
        // a x b = (a2b3-a3b2)i+(a3b1-a1b3)j+(a1b2-a2b1)k
        return {y * v.getZ() - z * v.getY(),
                z * v.getX() - x * v.getZ(),
                x * v.getY() - y * v.getX()};
    }

    inline float getX() const {
        return x;
    }

    inline float getY() const {
        return y;
    }

    inline float getZ() const {
        return z;
    }

    inline float getR() const {
        return x;
    }

    inline float getG() const {
        return y;
    }

    inline float getB() const {
        return z;
    }

    inline const vector3D &operator+() const {
        return *this;
    }

    inline vector3D operator-() const {
        return {-x, -y, -z};
    }

    inline float operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        throw std::out_of_range("vector3D : index " + std::to_string(i) + " is out of range");
    }

    inline float &operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        throw std::out_of_range("vector3D : index " + std::to_string(i) + " is out of range");
    }

    inline vector3D &operator+=(const vector3D &v) {
        x += v.getX();
        y += v.getY();
        z += v.getZ();
        return *this;
    }

    inline vector3D &operator-=(const vector3D &v) {
        x -= v.getX();
        y -= v.getY();
        z -= v.getZ();
        return *this;
    }

    inline vector3D &operator*=(const vector3D &v) {
        x *= v.getX();
        y *= v.getY();
        z *= v.getZ();
        return *this;
    }

    inline vector3D &operator/=(const vector3D &v) {
        x /= v.getX();
        y /= v.getY();
        z /= v.getZ();
        return *this;
    }

    inline vector3D &operator+=(const float k) {
        x += k;
        y += k;
        z += k;
        return *this;
    }

    inline vector3D &operator-=(const float k) {
        x -= k;
        y -= k;
        z -= k;
        return *this;
    }

    inline vector3D &operator*=(const float k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    inline vector3D &operator/=(const float k) {
        x /= k;
        y /= k;
        z /= k;
        return *this;
    }

    inline std::istream &operator>>(std::istream &is) {
        is >> x >> y >> z;
        return is;
    }

    inline std::ostream &operator<<(std::ostream &os) {
        os << x << " " << y << " " << z;
        return os;
    }

};

inline vector3D operator+(const vector3D &v1, const vector3D &v2) {
    return {v1.getX() + v2.getX(),
            v1.getY() + v2.getY(),
            v1.getZ() + v2.getZ()};
}

inline vector3D operator-(const vector3D &v1, const vector3D &v2) {
    return {v1.getX() - v2.getX(),
            v1.getY() - v2.getY(),
            v1.getZ() - v2.getZ()};
}

inline vector3D operator*(const vector3D &v1, const vector3D &v2) {
    return {v1.getX() * v2.getX(),
            v1.getY() * v2.getY(),
            v1.getZ() * v2.getZ()};
}

inline vector3D operator*(const float &c, const vector3D &v) {
    return {c * v.getX(), c * v.getY(), c * v.getZ()};
}

inline vector3D operator*(const vector3D &v, const float &c) {
    return {c * v.getX(), c * v.getY(), c * v.getZ()};
}

inline vector3D operator/(const vector3D &v1, const vector3D &v2) {
    return {v1.getX() / v2.getX(),
            v1.getY() / v2.getY(),
            v1.getZ() / v2.getZ()};
}

inline vector3D operator/(const vector3D &v, const float &c) {
    return {v.getX() / c, v.getY() / c, v.getZ() / c};
}


#endif //RAYTRACER_VECTOR3D_H

