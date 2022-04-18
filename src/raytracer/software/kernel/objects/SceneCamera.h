#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneCamera
		{
		public:
			SceneCamera() :
				m_Position(0,0,0), 
				m_Target(0,0,-1),
				m_Up(0,1,0),
				m_FOV(60),
				m_UseDepthOfDield(false)
			{}
			//--------------------------------
			inline void setView(vec3 pos, vec3 target)
			{
				m_Position = pos;
				m_Target = target;
			}
			inline void setView(vec3 pos,vec3 target,vec3 up)
			{
				setView(pos, target);
				m_Up = up;
			}
			inline void setView(vec3 pos, vec3 target, vec3 up, float fov)
			{
				setView(pos, target, up);
				m_FOV = fov;
			}
			inline void setView(vec3 pos, vec3 target, vec3 up, float fov, float aperture, float fdist)
			{
				setView(pos, target, up, fov);
				m_Aperture = aperture;
				m_FocusDist = fdist;
			}
			//--------------------------------
			void Build()
			{
				float theta = deg_to_rad(m_FOV);
				float h = ntan<float>(theta / 2.0f);
				float viewport_height = 2.0f * h;
				float viewport_width = m_Aspect * viewport_height;

				m_ForwardVector = normalize(m_Position - m_Target);
				m_HorizontalVector = normalize(cross(m_Up, m_ForwardVector));
				m_VerticalVector = -cross(m_ForwardVector, m_HorizontalVector);

				if (m_UseDepthOfDield)
				{
					m_UVVectors[0] = m_FocusDist * viewport_width * m_HorizontalVector;
					m_UVVectors[1] = m_FocusDist * viewport_height * m_VerticalVector;
					m_RefPoint = m_Position - m_UVVectors[0] / 2.f - m_UVVectors[1] / 2.f - m_FocusDist * m_ForwardVector;
					m_LensRadius = m_Aperture / 2.0f;
				}
				else
				{
					m_UVVectors[0] = viewport_width * m_HorizontalVector;
					m_UVVectors[1] = viewport_height * m_VerticalVector;
					m_RefPoint = m_Position - m_UVVectors[0] / 2.f - m_UVVectors[1] / 2.f - m_ForwardVector;
				}
			}
			//--------------------------------
			RayInfo CastRay(float u, float v)
			{
				if (m_UseDepthOfDield)
				{
					vec3 rd = m_LensRadius * random_vector_2d();
					vec3 offset = m_HorizontalVector * rd.x + m_VerticalVector * rd.y;
					return RayInfo(m_Position + offset, (m_RefPoint + u * m_UVVectors[0] + v * m_UVVectors[1] - m_Position - offset));
				}
				else
				{
					return RayInfo(m_Position, (m_RefPoint + u * m_UVVectors[0] + v * m_UVVectors[1] - m_Position));
				}
			}
			//--------------------------------
			inline void setPosition(vec3 const& pos) { m_Position = pos; }
			inline vec3 const& getPosition() const { return m_Position;	}
			inline void setTarget(vec3 const& tgt) { m_Target = tgt; }
			inline vec3 const& getTarget() const { return m_Target;	}
			inline void setUp(vec3 const& up) { m_Up = up; }
			inline vec3 const& getUp() const { return m_Up; }
			inline void setFOV(float f) { m_FOV = f; }
			inline float getFOV() const { return m_FOV; }
			inline void setAspect(float a) { m_Aspect = a; }
			inline float getAspect() const { return m_Aspect; }
			//--------------------------------
			inline void setUseDepthOfDield(bool d) { m_UseDepthOfDield = d; }
			inline bool getUseDepthOfDield() const { return m_UseDepthOfDield; }
			//--------------------------------
		private:
			vec3 m_Position;
			vec3 m_Target;
			vec3 m_Up;
			float m_FOV;
			float m_Aperture;
			float m_FocusDist;
			float m_Aspect;

			vec3 m_RefPoint;
			vec3 m_UVVectors[2];
			float m_LensRadius;
			vec3 m_ForwardVector;
			vec3 m_VerticalVector;
			vec3 m_HorizontalVector;

			bool m_UseDepthOfDield;
		};
	}
}