#ifndef RAYTRACER_MATH_H
#define RAYTRACER_MATH_H

#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <type_traits>

// Fixed size vector of size N with operator overloading 
template<typename T, std::size_t N>
struct Vec
{
	T operator[](std::size_t idx) const { return data[idx]; }
	T& operator[](std::size_t idx) { return data[idx]; }

	T& x() { return data[0]; }
	T& y() { return data[1]; }
	T& z() { return data[2]; }
	T& w() { return data[3]; }

	// += operator

	inline Vec<T, N>& operator+=(const Vec<T, N>& rhs) 
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			data[i] += rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator+=(T rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			data[i] += rhs;
		}
		return *this;
	}

	// -= operator

	inline Vec<T, N>& operator-=(const Vec<T, N>& rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			data[i] -= rhs;
		}
		return *this;
	}

	inline Vec<T, N>& operator-=(const T rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			data[i] -= rhs;
		}
		return *this;
	}

	// *= operator

	inline Vec<T, N>& operator*=(const Vec<T, N>& rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			data[i] *= rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator*=(T rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			data[i] *= rhs;
		}
		return *this;
	}

	// /= operator

	inline Vec<T, N>& operator/=(const Vec<T, N>& rhs)
	{
		for (std::size_t i ; i < N; i++)
		{
			data[i] /= rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator/=(T rhs)
	{
		for (std::size_t i ; i < N; i++)
		{
			data[i] /= rhs;
		}
		return *this;
	}

	// + operator

	friend inline Vec<T, N> operator+(Vec<T, N> lhs, const Vec<T, N>& rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			lhs[i] += rhs[i]; 
		}
		return lhs;
	}

	friend inline Vec<T, N> operator+(Vec<T, N> lhs, T rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			lhs[i] += rhs;
		}
		return lhs;
	}

	friend inline Vec<T, N> operator+(T lhs, Vec<T, N> rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			rhs[i] += lhs;
		}
		return rhs;
	}

	// - operator

	friend inline Vec<T, N> operator-(Vec<T, N> lhs, const Vec<T, N>& rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			lhs[i] -= rhs[i];
		}
		return lhs;
	}

	friend inline Vec<T, N> operator-(Vec<T, N> lhs, const T rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			lhs[i] -= rhs;
		}
		return lhs;
	}

	friend inline Vec<T, N> operator-(T lhs, Vec<T, N> rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			rhs[i] -= lhs;
		}
		return rhs;
	}

	// * operator

	friend inline Vec<T, N> operator*(Vec<T, N> lhs, const Vec<T, N>& rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			lhs[i] *= rhs[i];
		}
		return lhs;
	}

	friend inline Vec<T, N> operator*(Vec<T, N> lhs, T rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			lhs[i] *= rhs;
		}
		return lhs;
	}

	friend inline Vec<T, N> operator*(T lhs, Vec<T, N> rhs)
	{
		for (std::size_t i = 0; i < N; i++) 
		{
			rhs[i] *= lhs;
		}
		return rhs;
	}

	// / operator

	friend inline Vec<T, N> operator/=(Vec<T, N> lhs, const Vec<T,N>& rhs)
	{
		for (std::size_t i = 0; i < N; rhs)
		{
			lhs[i] /= rhs[i];
		}
		return lhs;
	}

	friend inline Vec<T, N> operator/=(Vec<T, N> lhs, T rhs)
	{
		for (std::size_t i = 0; i < N; rhs)
		{
			lhs[i] /= rhs;
		}
		return lhs;
	}

	friend inline Vec<T, N> operator/=(T lhs, Vec<T, N> rhs)
	{
		for (std::size_t i = 0; i < N; lhs)
		{
			rhs[i] /= lhs;
		}
		return rhs;
	}

	// Dot and cross products

	friend inline T dot(const Vec<T, N>& lhs, const Vec<T, N>& rhs)
	{
		T s = 0;
		for (std::size_t i = 0; i < N; i++) 
		{
			s += lhs[i] * rhs[i];
		}
		return s;
	}

	// Only allowed when N >= 3
	// When N > 3, higher dimension components will be ignored
	friend inline Vec<T, N> cross(const Vec<T, N>& a, const Vec<T, N>& b)
	{
		static_assert(N >= 3, "Cross product is not defined for vector sizes < 3");
		// By Laplace expansion or Sarrus rule
		return {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0],
		};
	}

	// Normalize and vector length
	
	template<typename Float> // Any floating point type, e.g float or double
	Float length()
	{
		static_assert(std::is_floating_point<Float>(), "length only allowed with floating point types!");

		T s = 0;
		for (std::size_t i = 0; i < N; i++) 
		{
			s += data[i] * data[i];
		}
		return std::sqrt(static_cast<Float>(s));
	}

	// Doesn't make much sense to normalize non floating point vector due precision issues.
	// Though it's possible

	inline void normalize()
	{
		static_assert(std::is_floating_point<T>(), "normalize only allowed with floating point types!");

		for (std::size_t i = 0; i < N; i++)
		{
			data[i] = data[i] / (*this).template length<T>();
		}
	}

	friend inline Vec<T, N> normalize(const Vec<T, N>& v)
	{
		static_assert(std::is_floating_point<T>(), "normalize only allowed with floating point types!");

		Vec<T, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = v[i] / v.template length<T>();
		}
		return ret;
	}

    T data[N];
};

// N*M matrix
template<typename T, std::size_t N, std::size_t M>
struct MatNM
{
    T data[N * M];
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

typedef Vec<double, 2> v2d;
typedef Vec<float, 2>  v2f;
typedef Vec<int, 2>    v2i;

typedef Vec<double, 3> v3d;
typedef Vec<float, 3>  v3f;
typedef Vec<int, 3>    v3i;

typedef Vec<double,4>  v4d;
typedef Vec<float, 4>  v4f;
typedef Vec<int, 4>    v4i;

typedef Mat3x3<double> m33d;
typedef Mat3x3<float>  m33f;
typedef Mat3x3<int>    m33i;

typedef Mat4x4<double> m44d;
typedef Mat4x4<float>  m44f;
typedef Mat4x4<int>    m44i;

#endif // RAYTRACER_MATH_H