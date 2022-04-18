#pragma once

#include "nmath_defines.hpp"
#include "nmath_types.hpp"

NMATH_NAMESPACE_BEGIN

//This can be handy if you are wish to use vector as color storage
#if defined(NMATH_COLOR_TYPES)

template<typename T>
using ncolor3 = nvec<3, T>;
using color3u = ncolor3<uint8_t>;
using color3f = ncolor3<float>;

#endif

NMATH_NAMESPACE_END