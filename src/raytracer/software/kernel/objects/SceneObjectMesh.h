#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneObjectMesh : public SceneObject
		{
		public:
			SceneObjectMesh() { };

			virtual bool CheckIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo) override
			{
				return false;
			}


			virtual AABB const& getAABB() const override
			{
				static AABB m_AABB(vec3(999999), vec3(-999999));
				return AABB();
			}

		};
	}
}