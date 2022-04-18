#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct RegionConfig
		{
			RegionConfig() = default;

			RegionConfig(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t s) :
				m_StartX(x), m_StartY(y),
				m_Width(w), m_Height(h),
				m_Samples(s)
			{};

			RegionConfig(const RegionConfig& t) :
				m_StartX(t.m_StartX), m_StartY(t.m_StartY),
				m_Width(t.m_Width), m_Height(t.m_Height),
				m_Samples(t.m_Samples)
			{};

			//--------------------------------
			uint32_t m_StartX; uint32_t m_StartY;
			uint32_t m_Width; uint32_t m_Height;
			uint32_t m_Samples;
			//--------------------------------
		};
	}
}