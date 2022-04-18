#include "nmath_vec3.hpp"

NMATH_NAMESPACE_BEGIN

template<typename T>
NMATH_FUNC_QUAL nvec<3, T>::nvec(nvec const& v) :
	x(v.x), y(v.y), z(v.z)
{
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T>::nvec(T s) :
	x(s), y(s), z(s)
{
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T>::nvec(T _x, T _y, T _z) :
	x(_x), y(_y), z(_z)
{
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>::nvec(N s) :
	x(static_cast<T>(s)), y(static_cast<T>(s)), z(static_cast<T>(s))
{
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>::nvec(N _x, N _y, N _z) :
	x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z))
{
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>::nvec(nvec<3, N> const& v) :
	x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z))
{
}

template<typename T>
NMATH_FUNC_QUAL T nvec<3, T>::operator[] (size_t i) const
{
	return c[i];
}

template<typename T>
NMATH_FUNC_QUAL T& nvec<3, T>::operator[] (size_t i)
{
	return c[i];
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator=(nvec const& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator=(nvec<3, N> const& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	z = static_cast<T>(v.z);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator=(N s)
{
	x = static_cast<T>(s);
	y = static_cast<T>(s);
	z = static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator+=(nvec<3, N> const& v)
{
	x += static_cast<T>(v.x);
	y += static_cast<T>(v.y);
	z += static_cast<T>(v.z);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator+=(N s)
{
	x += static_cast<T>(s);
	y += static_cast<T>(s);
	z += static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator-=(nvec<3, N> const& v)
{
	x -= static_cast<T>(v.x);
	y -= static_cast<T>(v.y);
	z -= static_cast<T>(v.z);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator-=(N s)
{
	x -= static_cast<T>(s);
	y -= static_cast<T>(s);
	z -= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator*=(nvec<3, N> const& v)
{
	x *= static_cast<T>(v.x);
	y *= static_cast<T>(v.y);
	z *= static_cast<T>(v.z);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator*=(N s)
{
	x *= static_cast<T>(s);
	y *= static_cast<T>(s);
	z *= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator/=(nvec<3, N> const& v)
{
	x /= static_cast<T>(v.x);
	y /= static_cast<T>(v.y);
	z /= static_cast<T>(v.z);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<3, T>& nvec<3, T>::operator/=(N s)
{
	x /= static_cast<T>(s);
	y /= static_cast<T>(s);
	z /= static_cast<T>(s);
	return *this;
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator+(nvec<3, T> const& v)
{
	return v;
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator-(nvec<3, T> const& v)
{
	return nvec<3, T>(-v.x, -v.y, -v.z);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator+(nvec<3, T> const& v1, nvec<3, T> const& v2)
{
	return nvec<3, T>(
		v1.x + v2.x,
		v1.y + v2.y,
		v1.z + v2.z
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator+(nvec<3, T> const& v, T s)
{
	return nvec<3, T>(
		v.x + s,
		v.y + s,
		v.z + s
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator+(T s, nvec<3, T> const& v)
{
	return nvec<3, T>(
		s + v.x,
		s + v.y,
		s + v.z
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator-(nvec<3, T> const& v1, nvec<3, T> const& v2)
{
	return nvec<3, T>(
		v1.x - v2.x,
		v1.y - v2.y,
		v1.z - v2.z
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator-(nvec<3, T> const& v, T s)
{
	return nvec<3, T>(
		v.x - s,
		v.y - s,
		v.z - s
		);

}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator-(T s, nvec<3, T> const& v)
{
	return nvec<3, T>(
		s - v.x,
		s - v.y,
		s - v.z
		);

}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator*(nvec<3, T> const& v1, nvec<3, T> const& v2)
{
	return nvec<3, T>(
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator*(nvec<3, T> const& v, T s)
{
	return nvec<3, T>(
		v.x * s,
		v.y * s,
		v.z * s
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator*(T s, nvec<3, T> const& v)
{
	return nvec<3, T>(
		s * v.x,
		s * v.y,
		s * v.z
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator/(nvec<3, T> const& v1, nvec<3, T> const& v2)
{
	return nvec<3, T>(
		v1.x / v2.x,
		v1.y / v2.y,
		v1.z / v2.z
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator/(nvec<3, T> const& v, T s)
{
	return nvec<3, T>(
		v.x / s,
		v.y / s,
		v.z / s
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<3, T> operator/(T s, nvec<3, T> const& v)
{
	return nvec<3, T>(
		s / v.x,
		s / v.y,
		s / v.z
		);
}

NMATH_NAMESPACE_END