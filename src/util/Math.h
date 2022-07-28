#ifndef RAYTRACER_MATH_H
#define RAYTRACER_MATH_H

#include <array>
#include "util/Types.h"

template<typename T>
struct v2
{
	T x;
	T y;
};

template<typename T>
struct v3
{
	v3() : x(0), y(0), z(0) {}
	v3(T x, T y, T z) : x(x), y(y), z(z) {}
	T x;
	T y;
	T z;
};

template<typename T>
struct v4
{
	v4() : x(0), y(0), z(0), w(0) {}
	v4(T x, T y, T z, T w) :x(x), y(y), z(y), w(w) {}
	T x;
	T y;
	T z;
	T w;
};

template<typename T, std::size_t N>
struct vec
{
	T operator[](u32 idx) const { return data[idx]; }
	T& operator[](u32 idx) { return data[idx]; }

	T& x() { return data[0]; }
	T& y() { return data[1]; }
	T& z() { return data[2]; }
	T& w() { return data[3]; }

	vec<T, N>& operator+=(const vec<T, N>& rhs) 
	{
		for (int i = 0; i < N; i++) 
		{
			data[i] += rhs[i];
		}
		return *this;
	}

	vec<T, N>& operator+=(T rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			data[i] += rhs;
		}
		return *this;
	}

	vec<T, N>& operator-=(const vec<T, N>& rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			data[i] -= rhs;
		}
		return *this;
	}

	vec<T, N>& operator-=(const T rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			data[i] -= rhs;
		}
		return *this;
	}

	vec<T, N>& operator*=(const vec<T, N>& rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			data[i] *= rhs[i];
		}
		return *this;
	}

	vec<T, N>& operator*=(T rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			data[i] *= rhs;
		}
		return *this;
	}

	friend vec<T, N> operator+(vec<T, N> lhs, const vec<T, N>& rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			lhs[i] += rhs[i]; 
		}
		return lhs;
	}

	friend vec<T, N> operator+(vec<T, N> lhs, T rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			lhs[i] += rhs;
		}
		return lhs;
	}

	friend vec<T, N> operator-(vec<T, N> lhs, const vec<T, N>& rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			lhs[i] -= rhs[i];
		}
		return lhs;
	}

	friend vec<T, N> operator-(vec<T, N> lhs, const T rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			lhs[i] -= rhs;
		}
		return lhs;
	}

	friend vec<T, N> operator*(vec<T, N> lhs, const vec<T, N>& rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			lhs[i] *= rhs[i];
		}
		return lhs;
	}

	friend vec<T, N> operator*(vec<T, N> lhs, T rhs)
	{
		for (int i = 0; i < N; i++) 
		{
			lhs[i] *= rhs;
		}
		return lhs;
	}

	friend T dot(const vec<T, N>& lhs, const vec<T, N>& rhs)
	{
		T s;
		for (int i = 0; i < N; i++) 
		{
			s += lhs[i] * rhs[i];
		}
		return s;
	}

	operator std::array<T, N>&() { return data; }
	std::array<T, N> data;
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

typedef vec<double, 2> v2d;
typedef vec<float, 2>  v2f;
typedef vec<int, 2>    v2i;

typedef v3<double> v3d;
typedef v3<float>  v3f;
typedef v3<int>    v3i;

typedef v4<double> v4d;
typedef v4<float>  v4f;
typedef v4<int>    v4i;

typedef Mat3x3<double> m33d;
typedef Mat3x3<float>  m33f;
typedef Mat3x3<int>    m33i;

typedef Mat4x4<double> m44d;
typedef Mat4x4<float>  m44f;
typedef Mat4x4<int>    m44i;

#endif // RAYTRACER_MATH_H