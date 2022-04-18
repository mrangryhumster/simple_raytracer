#pragma once
#include <cstdint>

namespace Raytracer
{
	struct RaytracerConfig
	{
		uint32_t m_OutputWidth, m_OutputHeight;
		uint32_t m_TileWidth, m_TileHeight;
		uint32_t m_Threads;
		uint32_t m_SamplesPerPixel;
		uint32_t m_RayMaxBounces;
		uint32_t m_RayMaxDiffuseBounces;
		uint32_t m_RayMaxReflectionBounces;
		uint32_t m_RayMaxRefractionBounces;

		uint32_t m_Internal_SoftwareBVHType;
	};
}