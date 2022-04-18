#pragma once

#include "SoftwareKernel.h"
#include <memory>

namespace Raytracer
{
	namespace Software
	{
		class SceneHierarchyBVH_RTIOW : public SceneHierarchy
		{
		public:
			SceneHierarchyBVH_RTIOW();
			virtual ~SceneHierarchyBVH_RTIOW() override;
			virtual void Clear() override;
			virtual void Build(SceneObject** sceneObjects, size_t sceneObjectsCount) override;
			virtual bool FindIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInf) override;

		private:

			SceneObject* m_RootNode;
		};
	}
}