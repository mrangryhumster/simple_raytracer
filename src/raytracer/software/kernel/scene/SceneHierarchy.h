#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneHierarchy
		{
		public:
			virtual ~SceneHierarchy() = default;
			//WARNING! : passed data could affected by sorting while internal processing
			virtual void Build(SceneObject** sceneObjects, size_t sceneObjectsCount) = 0;
			virtual void Clear() = 0;

			virtual bool FindIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo) = 0;
		};
	}
}