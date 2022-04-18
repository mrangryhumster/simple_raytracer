#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneObjectSphere : public SceneObject
		{
		public:
			SceneObjectSphere(vec3 _position, float _radius, MaterialObject* _material) :
				m_Position(_position), 
				m_Radius(_radius), 
				m_Material(_material),
				m_AABB(_position - _radius, _position + _radius)
			{};
			virtual ~SceneObjectSphere() override
			{};

			virtual bool CheckIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo) override
			{
				vec3 oc = rayInfo.m_Origin - m_Position;
				auto a = length_squared(rayInfo.m_Direction);
				auto half_b = dot(oc, rayInfo.m_Direction);
				auto c = length_squared(oc) - m_Radius * m_Radius;

				if (c > 0.0f && half_b > 0.0f) return 0;

				auto discriminant = half_b * half_b - a * c;
				if (discriminant < 0) return false;
				auto sqrtd = nsqrt<float>(discriminant);

				// Find the nearest root that lies in the acceptable range.
				auto root = (-half_b - sqrtd) / a;
				if (root < RAY_MIN_DISTANCE || rayInfo.m_Distance < root)
				{
					root = (-half_b + sqrtd) / a;
					if (root < RAY_MIN_DISTANCE || rayInfo.m_Distance < root)
						return false;
				}
				
				rayInfo.m_Distance = root;

				hitInfo.m_Position = rayInfo.m_Origin + (rayInfo.m_Direction * root);
				hitInfo.m_Distance = root;

				//Set normal direction
				hitInfo.setFaceNormal(rayInfo.m_Direction, ((hitInfo.m_Position - m_Position) / m_Radius));

				//Get UV coordinates
				auto norm = ((hitInfo.m_Position - m_Position) / m_Radius);
				auto theta = acos(-norm.y);
				auto phi = atan2(-norm.z, norm.x) + pi32;

				hitInfo.m_UV.x = phi / (2 * pi32);
				hitInfo.m_UV.y = theta / pi32;

				//Additional things
				hitInfo.m_SceneObject = this;
				hitInfo.m_Material = m_Material;

				return true;
			}

			virtual AABB const& getAABB() const override
			{
				return m_AABB;
			}


			AABB			m_AABB;
			vec3			m_Position;
			float			m_Radius;
			MaterialObject*	m_Material;
		};
	}
}