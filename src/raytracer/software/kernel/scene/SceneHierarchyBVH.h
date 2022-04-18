#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class SceneHierarchyBVH : public SceneHierarchy
		{
		public:
			SceneHierarchyBVH();
			virtual ~SceneHierarchyBVH() override;
			virtual void Clear() override;
			virtual void Build(SceneObject** sceneObjects, size_t sceneObjectsCount) override;
			virtual bool FindIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInf) override;

		private:

			//Declare internal structs
			struct __BVHNode;

			//Flattened tree
			__BVHNode* m_Nodes;
			SceneObject** m_SceneObjects;

			size_t ConstructFlatNode(size_t nodeIndex, __BVHNode* nodesArray, SceneObject** sceneObjects, size_t start, size_t end);
		};
	}
}