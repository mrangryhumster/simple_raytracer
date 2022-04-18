#pragma once

#include <cmath>
#include <limits>

NMATH_NAMESPACE_BEGIN

//Constants
#ifndef pi32
//float pi
constexpr const float pi32 = 3.14159265359f;
#endif // pi32

#ifndef pi64
//double pi64
constexpr const double pi64 = 3.1415926535897932384626433832795028841971693993751; //MOAR PI!!! MOAR !!!
#endif // pi64

#ifndef epsilon32
constexpr const float epsilon32 = std::numeric_limits<float>::epsilon();
#endif // epsilon32

#ifndef epsilon64
constexpr const double epsilon64 = std::numeric_limits<double>::epsilon();
#endif // epsilon64

#ifndef infinity32
constexpr const float infinity32 = std::numeric_limits<float>::infinity();
#endif // infinity32

#ifndef infinity64
constexpr const double infinity64 = std::numeric_limits<double>::infinity();
#endif // infinity64

//Scalar functions
// --- basic math ---
template<typename T>
NMATH_INLINE T nmin(T s1, T s2);
template<typename T>
NMATH_INLINE T nmax(T s1, T s2);
template<typename T>
NMATH_INLINE T nabs(T s);
template<typename T>
NMATH_INLINE T nfloor(T s);
template<typename T>
NMATH_INLINE T nceil(T s);
template<typename T>
NMATH_INLINE T nround(T s);
template<typename T>
NMATH_INLINE T npower(T s, T p);
// --- more math ---
template<typename T>
NMATH_INLINE T nsqrt(T s);
template<typename T>
NMATH_INLINE T ntan(T s);
template<typename T>
NMATH_INLINE T nsin(T s);
template<typename T>
NMATH_INLINE T ncos(T s);
template<typename T>
NMATH_INLINE T nlog2(T s);
// --- deg math ---
template<typename T>
NMATH_INLINE T deg_to_rad(T s);
// --- utils math ---
template<typename T, typename N>
NMATH_INLINE T map_value(N val, N min, N max);



//Vector math functions
// --- vec3 math ---
template<typename T>
NMATH_FUNC_DECL T length(nvec<3, T> const& v);
template<typename T>
NMATH_FUNC_DECL T length_squared(nvec<3, T> const& v);
template<typename T>
NMATH_FUNC_DECL T distance(nvec<3, T> const& v1, nvec<3, T> const& v2);
template<typename T>
NMATH_FUNC_DECL T distance_squared(nvec<3, T> const& v1, nvec<3, T> const& v2);
template<typename T>
NMATH_FUNC_DECL nvec<3, T> reflect(nvec<3, T> const& v, nvec<3, T> const& normal);
template<typename T>
NMATH_FUNC_DECL nvec<3, T> refract(nvec<3, T> const& v, nvec<3, T> const& normal, T ior);
template<typename T>
NMATH_FUNC_DECL nvec<3, T> normalize(nvec<3, T> const& v);
template<typename T>
NMATH_FUNC_DECL nvec<3, T> cross(nvec<3, T> v1, nvec<3, T> v2);
template<typename T>
NMATH_FUNC_DECL T dot(nvec<3, T> v1, nvec<3, T> v2);

NMATH_NAMESPACE_END


