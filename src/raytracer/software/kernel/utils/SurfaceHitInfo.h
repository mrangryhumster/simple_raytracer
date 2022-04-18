#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct SurfaceHitInfo
		{
		public:
			SurfaceHitInfo() = default;

			inline void setFaceNormal(vec3 const& hitDirection, vec3 const& objNormal)
			{
				m_IsFrontFace = dot(hitDirection, objNormal) < 0.0f;
				m_Normal = m_IsFrontFace ? objNormal : -objNormal;
			}

			vec3	m_Position;
			float	m_Distance;
			vec3	m_Normal;
			bool	m_IsFrontFace;
			vec2	m_UV;

			SceneObject* m_SceneObject;
			MaterialObject*		m_Material;
		};
	}
}