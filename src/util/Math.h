#ifndef RAYTRACER_MATH_H
#define RAYTRACER_MATH_H

#include <cstddef>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include "logging/Logging.h"
#include "util/Types.h"

// Header only, implements all the vector/matrix math 
// required by the application

// Using something like std::size_t is likely to be overkill
// since the rest of the application is mostly using vectors
// smaller or equal to 4.
typedef u32 Size;

// used for debugging
typedef std::string String;

// Fixed size vector of size N with operator overloading 
template<typename T, Size N>
struct Vec
{
	T operator[](Size idx) const { return data[idx]; }
	T& operator[](Size idx)      { return data[idx]; }

	T& x() { return data[0]; }
	T& y() { return data[1]; }
	T& z() { return data[2]; }
	T& w() { return data[3]; }

	// += operator

	inline Vec<T, N>& operator+=(const Vec<T, N>& rhs) 
	{
		for (Size i = 0; i < N; i++) 
		{
			data[i] += rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator+=(T rhs)
	{
		for (Size i = 0; i < N; i++) 
		{
			data[i] += rhs;
		}
		return *this;
	}

	// -= operator

	inline Vec<T, N>& operator-=(const Vec<T, N>& rhs)
	{
		for (Size i = 0; i < N; i++) 
		{
			data[i] -= rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator-=(const T rhs)
	{
		for (Size i = 0; i < N; i++) 
		{
			data[i] -= rhs;
		}
		return *this;
	}

	// *= operator

	inline Vec<T, N>& operator*=(const Vec<T, N>& rhs)
	{
		for (Size i = 0; i < N; i++) 
		{
			data[i] *= rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator*=(T rhs)
	{
		for (Size i = 0; i < N; i++) 
		{
			data[i] *= rhs;
		}
		return *this;
	}

	// /= operator

	inline Vec<T, N>& operator/=(const Vec<T, N>& rhs)
	{
		for (Size i ; i < N; i++)
		{
			data[i] /= rhs[i];
		}
		return *this;
	}

	inline Vec<T, N>& operator/=(T rhs)
	{
		for (Size i ; i < N; i++)
		{
			data[i] /= rhs;
		}
		return *this;
	}

	// + operator

	friend inline Vec<T, N> operator+(Vec<T, N> lhs, const Vec<T, N>& rhs)
	{
		return lhs += rhs;
	}

	friend inline Vec<T, N> operator+(Vec<T, N> lhs, T rhs)
	{
		return lhs += rhs;
	}

	friend inline Vec<T, N> operator+(T lhs, Vec<T, N> rhs)
	{
		return rhs += lhs;
	}

	// - operator

	friend inline Vec<T, N> operator-(Vec<T, N> lhs, const Vec<T, N>& rhs)
	{
		return lhs -= rhs;
	}

	friend inline Vec<T, N> operator-(Vec<T, N> lhs, const T rhs)
	{
		return lhs -= rhs;
	}

	friend inline Vec<T, N> operator-(T lhs, Vec<T, N> rhs)
	{
		return rhs -= lhs;
	}

	// * operator

	friend inline Vec<T, N> operator*(Vec<T, N> lhs, const Vec<T, N>& rhs)
	{
		return lhs *= rhs;
	}

	friend inline Vec<T, N> operator*(Vec<T, N> lhs, T rhs)
	{
		return lhs *= rhs;
	}

	friend inline Vec<T, N> operator*(T lhs, Vec<T, N> rhs)
	{
		return rhs *= lhs;
	}

	// / operator

	friend inline Vec<T, N> operator/=(Vec<T, N> lhs, const Vec<T,N>& rhs)
	{
		return lhs /= rhs;
	}

	friend inline Vec<T, N> operator/=(Vec<T, N> lhs, T rhs)
	{
		return lhs /= rhs;
	}

	friend inline Vec<T, N> operator/=(T lhs, Vec<T, N> rhs)
	{
		return rhs /= lhs;
	}

	// Dot and cross products

	friend inline T dot(const Vec<T, N>& lhs, const Vec<T, N>& rhs)
	{
		T s = 0;
		for (Size i = 0; i < N; i++)
		{
			s += lhs[i] * rhs[i];
		}
		return s;
	}

	// Only allowed when N >= 3
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
		static_assert(std::is_floating_point<Float>(), "length() only allowed with floating point types!");

		T s = 0;
		for (Size i = 0; i < N; i++) 
		{
			s += data[i] * data[i];
		}
		return std::sqrt(static_cast<Float>(s));
	}

	inline void normalize()
	{
		static_assert(std::is_floating_point<T>(), "normalize() only allowed with floating point types!");

		for (Size i = 0; i < N; i++)
		{
			data[i] = data[i] / (*this).template length<T>();
		}
	}

	friend inline Vec<T, N> normalize(const Vec<T, N>& v)
	{
		static_assert(std::is_floating_point<T>(), "normalize(const Vec<T, N>& v) only allowed with floating point types!");

		Vec<T, N> ret;
		for (Size i = 0; i < N; i++)
		{
			ret[i] = v[i] / v.template length<T>();
		}
		return ret;
	}

	// for debugging
	inline String to_string()
	{
		String s = "(";
		for (Size i = 0; i < N; i++)
		{
			s += std::to_string(data[i]);
			if (i != N - 1)
				s += ", ";
		}
		s += ")";
		return s;
	}

    T data[N];
};

// Matrix with M rows and N columns
template<typename T, Size M, Size N>
struct Matrix
{
	T operator[](Size idx) const { return data[idx]; }
	T& operator[](Size idx)      { return data[idx]; }

	inline Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& rhs)
	{
		for (Size i = 0; i < M * N; i++)
		{
			data[i] += rhs[i];
		}
		return *this;
	}

	inline Matrix<T, M, N>& operator+=(const T rhs)
	{
		for (Size i = 0; i < N; i++)
		{
			data[i] += rhs;
		}
		return *this;
	}

	inline Matrix<T, M, N>& operator-=(const Matrix<T, M, N>& rhs)
	{
		for (Size i = 0; i < N * M; i++)
		{
			data[i] -= rhs[i];
		}
		return *this;
	}

	inline Matrix<T, M, N>& operator-=(const T rhs)
	{
		for (Size i = 0; i < M * N; i++)
		{
			data[i] -= rhs;
		}
		return *this;
	}

	friend inline Matrix<T, N, M> transpose(const Matrix<T, M, N>& data) 
	{
		Matrix<T, N, M> m;
		for (Size i = 0; i < M; i++)
		{
			for (Size j = 0; j < N; j++)
			{
				m[j * M + i] = data[i * N + j];
			}
		}
		return m;
	}

	// Matrix multiplication is only defined when the left-hand matrix has the same number of columns as
	// the right-hand matrix has rows, hence only one additional template parameter is required.
	template<Size P> friend inline Matrix<T, N, P> operator*(const Matrix<T, N, M>& lhs, const Matrix<T, M, P>& rhs) 
	{
		auto rhs_t = transpose(rhs);

		Matrix<T, N, P> m = {0};
		for (Size i = 0; i < N; i++)
		{
			for(Size j = 0; j < P; j++)
			{
				for (Size k = 0; k < M; k++)
				{
					m[i * P + j] += lhs[i * M + k] * rhs_t[j * M + k];
				}
			}
		}
	}

	// for debugging
	inline String to_string()
	{
		String s = "[\n";
		for (Size i = 0; i < N * M; i++)
		{
			s +=std::to_string(data[i]);
			if ((i + 1) % N == 0 && i > 0)
				s += "\n";
			else
				s +=", ";
		}
		s += "]";
		return s;
	}

    T data[N * M];
};

// cameraTransform gives cameras position and orientation.
// The view matrix is the inverse of the cameraTransform matrix.
// Since cameraTransform behaves "nicely", only rotates and 
// transforms, the inverse can be reduced to only transposing 
// the upper left 3x3 matrix and negating the position vector.
template<typename T>
Matrix<T, 4, 4> view(const Matrix<T, 4, 4>& cameraTransform)
{
	// To transpose the upper 3x3 matrix
	auto camera_t = transpose(cameraTransform);

	// After transposing the position vector can be found in
	// the last row of camera_t

	// Looping through the last row of camera_t i.e the position vector
	Size j = 3;
	for (Size i  = 12; i < 16; i++)
	{
		// Move last row to last column and negate
		camera_t[j] = -camera_t[i];
		j += 4;
	}

	return camera_t;
}

typedef Vec<double, 2>       v2d;
typedef Vec<float, 2>        v2f;
typedef Vec<i32, 2>          v2i32;
typedef Vec<u32, 2>          v2u32;
typedef Vec<i64, 2>          v2i64;
typedef Vec<u64, 2>          v2u64;

typedef Vec<double, 3>       v3d;
typedef Vec<float, 3>        v3f;

typedef Vec<i32, 3>          v3i32;
typedef Vec<double,4>        v4d;
typedef Vec<float, 4>        v4f;
typedef Vec<i32, 4>          v4i32;

typedef Matrix<double, 3, 3> m33d;
typedef Matrix<float, 3, 3>  m33f;
typedef Matrix<int, 3, 3>    m33i;

typedef Matrix<double, 4, 4> m44d;
typedef Matrix<float, 4, 4>  m44f;
typedef Matrix<int, 4, 4>    m44i;

#endif // RAYTRACER_MATH_H