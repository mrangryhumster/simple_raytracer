#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class AABB
		{
		public:
			AABB() = default;
			AABB(vec3 const& minimum, vec3 const& maximum) : m_Minimum(minimum), m_Maximum(maximum) {};

			inline bool CheckIntersection(const RayInfo& rayInfo) const
			{
				float t_min = 0;
				float t_max = rayInfo.m_Distance;
				for (size_t i = 0; i < 3; i++) 
				{
					float t0 = (m_Minimum[i] - rayInfo.m_Origin[i]) * rayInfo.m_InverseDirection[i];
					float t1 = (m_Maximum[i] - rayInfo.m_Origin[i]) * rayInfo.m_InverseDirection[i];
					if (rayInfo.m_InverseDirection[i] < 0.0f) std::swap(t0, t1);
					t_min = t0 > t_min ? t0 : t_min;
					t_max = t1 < t_max ? t1 : t_max;
					if (t_max <= t_min) return false;
				}
				return true;
			}

			inline vec3 getDiminsions()
			{
				return m_Maximum - m_Minimum;
			}

			inline void Add(vec3 const& p)
			{
				m_Minimum[0] = nmin<float>(m_Minimum[0], p[0]);
				m_Minimum[1] = nmin<float>(m_Minimum[1], p[1]);
				m_Minimum[2] = nmin<float>(m_Minimum[2], p[2]);

				m_Maximum[0] = nmax<float>(m_Maximum[0], p[0]);
				m_Maximum[1] = nmax<float>(m_Maximum[1], p[1]);
				m_Maximum[2] = nmax<float>(m_Maximum[2], p[2]);
			}

			inline void Add(AABB const& b)
			{
				m_Minimum[0] = nmin<float>(m_Minimum[0], b.m_Minimum[0]);
				m_Minimum[1] = nmin<float>(m_Minimum[1], b.m_Minimum[1]);
				m_Minimum[2] = nmin<float>(m_Minimum[2], b.m_Minimum[2]);

				m_Maximum[0] = nmax<float>(m_Maximum[0], b.m_Maximum[0]);
				m_Maximum[1] = nmax<float>(m_Maximum[1], b.m_Maximum[1]);
				m_Maximum[2] = nmax<float>(m_Maximum[2], b.m_Maximum[2]);
			}

			inline static AABB SurroundingAABB(AABB const& a, AABB const& b)
			{
				return AABB(
					vec3(
						nmin<float>(a.m_Minimum[0], b.m_Minimum[0]),
						nmin<float>(a.m_Minimum[1], b.m_Minimum[1]),
						nmin<float>(a.m_Minimum[2], b.m_Minimum[2])
					),
					vec3(
						nmax<float>(a.m_Maximum[0], b.m_Maximum[0]),
						nmax<float>(a.m_Maximum[1], b.m_Maximum[1]),
						nmax<float>(a.m_Maximum[2], b.m_Maximum[2])
					)
				);
			}

			vec3 m_Minimum;
			vec3 m_Maximum;
		};
	}
}