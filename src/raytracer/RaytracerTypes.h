#pragma once

#include <cstdint>
#include <memory>

#include "nmath.hpp"

namespace Raytracer
{
	enum class TexturePixelFormat
	{
		RGB,
		RGBA,
		BGR,
		BGRA,
		DEPTH
	};

	enum class TexturePixelType
	{
		UINT8,
		UINT32,
		FLOAT
	};

}