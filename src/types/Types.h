#ifndef RAYTRACER_TYPES_H
#define RAYTRACER_TYPES_H

template<typename T>
struct Vec2
{
	T x;
	T y;
};

template<typename T>
struct Vec3
{
	Vec3() : x(0), y(0), z(0) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
	T x;
	T y;
	T z;
};

template<typename T>
struct Vec4
{
	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(T x, T y, T z, T w) :x(x), y(y), z(y), w(w) {}
	T x;
	T y;
	T z;
	T w;
};

template<typename T>
struct Mat3x3
{
	T data[9];
	T& operator()() { return data; }
};

template<typename T>
struct Mat4x4
{
	T data[16];
	T& operator()() { return data; }
};

typedef Vec2<double> Vec2d;
typedef Vec2<float>  Vec2f;
typedef Vec2<int>    Vec2i;

typedef Vec3<double> Vec3d;
typedef Vec3<float>  Vec3f;
typedef Vec3<int>    Vec3i;

typedef Vec4<double> Vec4d;
typedef Vec4<float>  Vec4f;
typedef Vec4<int>    Vec4i;

typedef Mat3x3<double> Mat3x3d;
typedef Mat3x3<float>  Mat3x3f;
typedef Mat3x3<int>    Mat3x3i;

typedef Mat4x4<double> Mat4x4d;
typedef Mat4x4<float>  Mat4x4f;
typedef Mat4x4<int>    Mat4x4i;

#endif // RAYTRACER_TYPES_H
