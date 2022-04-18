#pragma once

#include <cstdint>
#include "nmath_defines.hpp"

NMATH_NAMESPACE_BEGIN

//Main vector template
template<int S, typename T> struct nvec;

//Default typedefs for 3d vector 

// --- 3d vector template ---
template <typename T = NMATH_DEFAULT_TYPE>
using nvec3 = nvec<3, T>;
// --- 3d vector default type ---
using vec3 = nvec<3, NMATH_DEFAULT_TYPE>;
// --- 3d vector additional types ---
using vec3f = nvec<3, float>;
using vec3d = nvec<3, double>;

// --- 2d vector template ---
template <typename T = NMATH_DEFAULT_TYPE>
using nvec2 = nvec<2, T>;
// --- 2d vector default type ---
using vec2 = nvec<2, NMATH_DEFAULT_TYPE>;
// --- 2d vector additional types ---
using vec2f = nvec<2, float>;
using vec2d = nvec<2, double>;

NMATH_NAMESPACE_END

#include "nmath_extensions.hpp"