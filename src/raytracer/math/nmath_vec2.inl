#include "nmath_vec2.hpp"

NMATH_NAMESPACE_BEGIN

template<typename T>
NMATH_FUNC_QUAL nvec<2, T>::nvec(nvec const& v) :
	x(v.x), y(v.y)
{
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T>::nvec(T s) :
	x(s), y(s)
{
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T>::nvec(T _x, T _y) :
	x(_x), y(_y)
{
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>::nvec(N s) :
	x(static_cast<T>(s)), y(static_cast<T>(s))
{
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>::nvec(N _x, N _y) :
	x(static_cast<T>(_x)), y(static_cast<T>(_y))
{
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>::nvec(nvec<2, N> const& v) :
	x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
{
}

template<typename T>
NMATH_FUNC_QUAL T nvec<2, T>::operator[] (size_t i) const
{
	return c[i];
}

template<typename T>
NMATH_FUNC_QUAL T& nvec<2, T>::operator[] (size_t i)
{
	return c[i];
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator=(nvec const& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator=(nvec<2, N> const& v)
{
	x = static_cast<T>(v.x);
	y = static_cast<T>(v.y);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator=(N s)
{
	x = static_cast<T>(s);
	y = static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator+=(nvec<2, N> const& v)
{
	x += static_cast<T>(v.x);
	y += static_cast<T>(v.y);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator+=(N s)
{
	x += static_cast<T>(s);
	y += static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator-=(nvec<2, N> const& v)
{
	x -= static_cast<T>(v.x);
	y -= static_cast<T>(v.y);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator-=(N s)
{
	x -= static_cast<T>(s);
	y -= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator*=(nvec<2, N> const& v)
{
	x *= static_cast<T>(v.x);
	y *= static_cast<T>(v.y);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator*=(N s)
{
	x *= static_cast<T>(s);
	y *= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator/=(nvec<2, N> const& v)
{
	x /= static_cast<T>(v.x);
	y /= static_cast<T>(v.y);
	return *this;
}

template<typename T>
template<typename N>
NMATH_FUNC_QUAL nvec<2, T>& nvec<2, T>::operator/=(N s)
{
	x /= static_cast<T>(s);
	y /= static_cast<T>(s);
	return *this;
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator+(nvec<2, T> const& v)
{
	return v;
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator-(nvec<2, T> const& v)
{
	return nvec<2, T>(-v.x, -v.y);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator+(nvec<2, T> const& v1, nvec<2, T> const& v2)
{
	return nvec<2, T>(
		v1.x + v2.x,
		v1.y + v2.y
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator+(nvec<2, T> const& v, T s)
{
	return nvec<2, T>(
		v.x + s,
		v.y + s
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator+(T s, nvec<2, T> const& v)
{
	return nvec<2, T>(
		s + v.x,
		s + v.y
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator-(nvec<2, T> const& v1, nvec<2, T> const& v2)
{
	return nvec<2, T>(
		v1.x - v2.x,
		v1.y - v2.y
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator-(nvec<2, T> const& v, T s)
{
	return nvec<2, T>(
		v.x - s,
		v.y - s
		);

}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator-(T s, nvec<2, T> const& v)
{
	return nvec<2, T>(
		s - v.x,
		s - v.y
		);

}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator*(nvec<2, T> const& v1, nvec<2, T> const& v2)
{
	return nvec<2, T>(
		v1.x * v2.x,
		v1.y * v2.y
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator*(nvec<2, T> const& v, T s)
{
	return nvec<2, T>(
		v.x * s,
		v.y * s
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator*(T s, nvec<2, T> const& v)
{
	return nvec<2, T>(
		s * v.x,
		s * v.y
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator/(nvec<2, T> const& v1, nvec<2, T> const& v2)
{
	return nvec<2, T>(
		v1.x / v2.x,
		v1.y / v2.y
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator/(nvec<2, T> const& v, T s)
{
	return nvec<2, T>(
		v.x / s,
		v.y / s
		);
}

template<typename T>
NMATH_FUNC_QUAL nvec<2, T> operator/(T s, nvec<2, T> const& v)
{
	return nvec<2, T>(
		s / v.x,
		s / v.y
		);
}

NMATH_NAMESPACE_END