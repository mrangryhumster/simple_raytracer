#pragma once

#include "nmath_func.hpp"

NMATH_NAMESPACE_BEGIN

//Basic functions
template<typename T>
NMATH_INLINE T nmin(T s1, T s2)
{
	return (s1 < s2) ? s1 : s2;
}

template<typename T>
NMATH_INLINE T nmax(T s1, T s2)
{
	return (s1 > s2) ? s1 : s2;
}

template<typename T>
NMATH_INLINE T nabs(T s)
{
	return static_cast<T>(std::abs(s));
}

template<typename T>
NMATH_INLINE T nfloor(T s)
{
	return static_cast<T>(std::floor(s));
}

template<typename T>
NMATH_INLINE T nceil(T s)
{
	return static_cast<T>(std::ceil(s));
}

template<typename T>
NMATH_INLINE T nround(T s)
{
	return static_cast<T>(std::round(s));
}

template<typename T>
NMATH_INLINE T npower(T s, T p)
{
	return static_cast<T>(std::pow(s, p));
}

template<typename T>
NMATH_INLINE T nsqrt(T s)
{
	return static_cast<T>(std::sqrt(s));
}

template<typename T>
NMATH_INLINE T ntan(T s)
{
	return static_cast<T>(std::tan(s));
}

template<typename T>
NMATH_INLINE T nsin(T s)
{
	return static_cast<T>(std::sin(s));
}

template<typename T>
NMATH_INLINE T ncos(T s)
{
	return static_cast<T>(std::cos(s));
}

template<typename T>
NMATH_INLINE T nlog2(T s)
{
	return static_cast<T>(std::log2(s));
}

template<typename T>
NMATH_INLINE T deg_to_rad(T s)
{
	return static_cast<T>(pi64 / 180.0 * s);
}

template<typename T, typename N>
NMATH_INLINE T map_value(N val, N min, N max)
{
	return (static_cast<T>(val) - static_cast<T>(min)) / (static_cast<T>(max) - static_cast<T>(min));
}

// --- vec3 math ---
template<typename T>
NMATH_FUNC_QUAL T length(nvec<3, T> const& v)
{
	return nsqrt<T>(static_cast<T>(v.x * v.x + v.y * v.y + v.z * v.z));
}

template<typename T>
NMATH_FUNC_QUAL T length_squared(nvec<3, T> const& v)
{
	return static_cast<T>(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<typename T>
NMATH_FUNC_QUAL T distance(nvec<3, T> const& v1, nvec<3, T> const& v2)
{
	return length<T>(v2 - v1);
}

template<typename T>
NMATH_FUNC_QUAL T distance_squared(nvec<3, T> const& v1, nvec<3, T> const& v2)
{
	return length_squared<T>(v2 - v1);
}

template<typename T>
NMATH_FUNC_DECL nvec<3, T> reflect(nvec<3, T> const& v, nvec<3, T> const& n)
{
	return v - 2 * dot(v, n) * n;
}

template<typename T>
NMATH_FUNC_DECL nvec<3, T> refract(nvec<3, T> const& v, nvec<3, T> const& n, T ior)
{
	T cos_theta = nmin<T>(dot(-v, n), static_cast<float>(1.0));
	nvec<3, T> pr_n = ior * (v + cos_theta * n);
	nvec<3, T> pl_n = -nsqrt<T>(nabs<T>(static_cast<float>(1.0) - length_squared<T>(pr_n))) * n;
	return pr_n + pl_n;
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> normalize(nvec<3, T> const& v)
{
	return nvec<3, T>(v / length<T>(v));
}
template<typename T>
NMATH_FUNC_QUAL nvec<3, T> cross(nvec<3, T> v1, nvec<3, T> v2)
{
	return nvec<3, T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
		);
}
template<typename T>
NMATH_FUNC_DECL T dot(nvec<3, T> v1, nvec<3, T> v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

NMATH_NAMESPACE_END