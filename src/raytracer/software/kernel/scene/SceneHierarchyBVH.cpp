#include "SceneHierarchyBVH.h"

namespace Raytracer
{
	namespace Software
	{
		//----------------------------------------------------------------

		struct SceneHierarchyBVH::__BVHNode
		{
			AABB m_AABB;
			uint32_t m_FarOffset;
			bool m_IsObjectNode;
			uint8_t _padding[3];
		};

		//----------------------------------------------------------------

		SceneHierarchyBVH::SceneHierarchyBVH() :
			m_Nodes(nullptr),
			m_SceneObjects(nullptr)
		{
		}

		SceneHierarchyBVH::~SceneHierarchyBVH()  
		{
			Clear();
		}

		void SceneHierarchyBVH::Clear()  
		{
			if (m_Nodes != nullptr)
			{
				delete[] m_Nodes;
				m_Nodes = nullptr;
				m_SceneObjects = nullptr;
			}
		}

		void SceneHierarchyBVH::Build(SceneObject** sceneObjects, size_t sceneObjectsCount)
		{
			Clear();
			if (sceneObjectsCount > 0)
			{
				size_t initialSize = static_cast<size_t>(npower(2.f, (nlog2((float)sceneObjectsCount) + 1)));
				m_Nodes = new __BVHNode[initialSize];
				ConstructFlatNode(0, m_Nodes, sceneObjects, 0, sceneObjectsCount);
				m_SceneObjects = sceneObjects;
			}
		}

		bool SceneHierarchyBVH::FindIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo)
		{
			if (m_Nodes == nullptr) return false;

			size_t stack[64];
			size_t stackIndex = 1; //Starts from one because 0 is exit condition
			size_t nodeIndex = 0;
			bool hit = false;
			do
			{
				const __BVHNode& node = m_Nodes[nodeIndex];

				if (node.m_IsObjectNode)
				{
					hit |= (m_SceneObjects[node.m_FarOffset]->CheckIntersection(rayInfo, hitInfo));
					nodeIndex = stack[--stackIndex];
				}
				else if(node.m_AABB.CheckIntersection(rayInfo))
				{
					nodeIndex++;
					stack[stackIndex++] = node.m_FarOffset;
				}
				else
				{
					//it's ok if we access uninitialized element at 0 index, because we anyway already finished we travesring
					nodeIndex = stack[--stackIndex];
				}
			} while (stackIndex > 0);

			return hit;
		}

		size_t SceneHierarchyBVH::ConstructFlatNode(size_t nodeIndex, SceneHierarchyBVH::__BVHNode* nodesArray, SceneObject** sceneObjects, size_t start, size_t end)
		{
			const size_t rangeSize = end - start;
			if (rangeSize > 1)
			{
				size_t splitAxis = 0;
				//Calculate aabb for this node
				AABB localAABB(sceneObjects[start]->getAABB());
				for (size_t objIndex = start + 1; objIndex < end; objIndex++)
					localAABB.Add(sceneObjects[objIndex]->getAABB());

				//Find longest axis of local aabb
				vec3 localDimension = localAABB.getDiminsions();
				if (localDimension[splitAxis] < localDimension[1]) splitAxis = 1;
				if (localDimension[splitAxis] < localDimension[2]) splitAxis = 2;

				//Calc range midle
				size_t mid = start + (rangeSize / 2);

				//Sort range using splitAxis as guide
				std::partial_sort(sceneObjects + start, sceneObjects + mid, sceneObjects + end,
					[&](SceneObject const* a, SceneObject const* b)
					{
						return a->getAABB().m_Minimum[splitAxis] < b->getAABB().m_Minimum[splitAxis];
					});

				size_t rightIndex = ConstructFlatNode(nodeIndex+1,nodesArray,sceneObjects, start, mid); // left
				size_t nextIndex = ConstructFlatNode(rightIndex, nodesArray,sceneObjects, mid, end); // right

				__BVHNode& node = nodesArray[nodeIndex];
				node.m_AABB = localAABB;
				node.m_FarOffset = static_cast<uint32_t>(rightIndex);
				node.m_IsObjectNode = false;
				return nextIndex;
			}
			else
			{
				__BVHNode& node = nodesArray[nodeIndex];
				node.m_AABB = sceneObjects[start]->getAABB();
				node.m_FarOffset = static_cast<uint32_t>(start);
				node.m_IsObjectNode = true;
				return nodeIndex + 1;
			}
		}
	}
}