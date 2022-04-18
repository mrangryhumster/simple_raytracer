#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneObject
		{
		public:

			virtual ~SceneObject() = default;
			virtual bool CheckIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo) = 0;
			virtual AABB const& getAABB() const = 0;

		};
	}
}