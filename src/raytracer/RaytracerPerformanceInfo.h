#pragma once

#include <cstdint>

namespace Raytracer
{
	struct RaytracerPerformanceInfo
	{
		struct StageTime
		{
			float m_SynchronizationTime;
			float m_RenderingTime;
			float m_FinalizationTime;
			float m_TotalTime;
		}m_StageTime;
	};
}