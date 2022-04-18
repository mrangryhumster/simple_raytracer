#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct RayInfo
		{
		public:
			RayInfo() = default;
			RayInfo(vec3 rayOrigin, vec3 rayDirection) :
				m_Origin(rayOrigin),
				m_Direction(rayDirection),
				m_InverseDirection(1.0f / rayDirection),
				m_Distance(RAY_MAX_DISTANCE)
			{}

			vec3 m_Origin;
			vec3 m_Direction;
			vec3 m_InverseDirection;
			float m_Distance;
		};
	}
}