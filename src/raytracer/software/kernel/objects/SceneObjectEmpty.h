#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneObjectEmpty : public SceneObject
		{
		public:
			SceneObjectEmpty() { printf("empty created!\n"); };

			virtual bool CheckIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo) override
			{
				return false;
			}


			virtual AABB const& getAABB() const override
			{
				//Define aabb as static will save us memory if there a lot of empty objects
				static AABB m_AABB(vec3(999999), vec3(-999999));
				return AABB();
			}
			
		};
	}
}