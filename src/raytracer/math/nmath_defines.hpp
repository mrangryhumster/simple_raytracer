#pragma once

#include "nmath_config.hpp"

#define NMATH_CONSTEXPR constexpr
#define NMATH_INLINE inline

#define NMATH_FUNC_DECL NMATH_CONSTEXPR
#define NMATH_FUNC_QUAL NMATH_INLINE NMATH_CONSTEXPR

#if !defined(NMATH_DEFAULT_TYPE)
#	define NMATH_DEFAULT_TYPE float
#endif // !NMATH_DEFAULT_TYPE

#if defined(NMATH_NAMESPACE_BEGIN_OVERRIDE) && defined(NMATH_NAMESPACE_END_OVERRIDE)

#	define NMATH_NAMESPACE_BEGIN NMATH_NAMESPACE_BEGIN_OVERRIDE
#	define NMATH_NAMESPACE_END NMATH_NAMESPACE_END_OVERRIDE
	//Little empty namespace just to be safe
	namespace nova { namespace math { } }
#else

#	define NMATH_NAMESPACE_BEGIN namespace nova { namespace math {
#	define NMATH_NAMESPACE_END }}

#endif // NMATH_NAMESPACE_BEGIN_OVERRIDE and NMATH_NAMESPACE_END_OVERRIDE


