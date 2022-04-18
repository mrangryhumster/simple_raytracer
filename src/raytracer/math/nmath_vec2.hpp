#pragma once

#include "nmath_defines.hpp"
#include "nmath_types.hpp"

NMATH_NAMESPACE_BEGIN

template <typename T>
struct nvec<2, T>
{
    using type = T;
    // --- Components ---
    union 
    {
        struct { T x, y; };
        struct { T r, g; };
        struct { T c[2]; };
    };
    // --- Constructors ---
    NMATH_FUNC_DECL nvec() = default;
    NMATH_FUNC_DECL nvec(T s);
    NMATH_FUNC_DECL nvec(T _x, T _y);
    NMATH_FUNC_DECL nvec(nvec const& v);
    // --- Constructors with conversion ---
    template<typename N>
    NMATH_FUNC_DECL nvec(N s);
    template<typename N>
    NMATH_FUNC_DECL nvec(N _x, N _y);
    template<typename N>
    NMATH_FUNC_DECL nvec(nvec<2, N> const& v);
    // --- Operators ---

    // Components
    NMATH_FUNC_DECL T operator[] (size_t i) const;
    NMATH_FUNC_DECL T& operator[] (size_t i);

    // Assignment operators
    NMATH_FUNC_DECL nvec<2, T>& operator= (nvec const& v);
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator= (nvec<2, N> const& v);
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator= (N s);

    // Arithmetic assignment operators
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator+= (nvec<2, N> const& v);
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator+= (N s);

    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator-= (nvec<2, N> const& v);
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator-= (N s);

    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator*= (nvec<2, N> const& v);
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator*= (N s);

    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator/= (nvec<2, N> const& v);
    template<typename N>
    NMATH_FUNC_DECL nvec<2, T>& operator/= (N s);

};

// Unary operators
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator+ (nvec<2, T> const& v);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator- (nvec<2, T> const& v);

template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator+ (nvec<2, T> const& v1, nvec<2, T> const& v2);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator+ (nvec<2, T> const& v, T s);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator+ (T s, nvec<2, T> const& v);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator- (nvec<2, T> const& v1, nvec<2, T> const& v2);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator- (nvec<2, T> const& v, T s);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator- (T s, nvec<2, T> const& v);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator* (nvec<2, T> const& v1, nvec<2, T> const& v2);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator* (nvec<2, T> const& v, T s);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator* (T s, nvec<2, T> const& v);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator/ (nvec<2, T> const& v1, nvec<2, T> const& v2);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator/ (nvec<2, T> const& v, T s);
template <typename T>
NMATH_FUNC_DECL nvec<2, T> operator/ (T s, nvec<2, T> const& v);

NMATH_NAMESPACE_END