#include "SceneHierarchyBVH_RTIOW.h"

namespace Raytracer
{
	namespace Software
	{
		//----------------------------------------------------------------
 
		//Helper class based on SceneObject;
		class SceneObjectBVHNode : public SceneObject
		{
		public:
			SceneObjectBVHNode(SceneObject** sceneObjects, size_t start, size_t end);

			virtual ~SceneObjectBVHNode() override;

			virtual bool CheckIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo) override;
			virtual AABB const& getAABB() const override;

			AABB m_AABB;
			bool m_IsFinalNode;
			SceneObject* m_LeftChild;
			SceneObject* m_RightChild;
		};

		//----------------------------------------------------------------

		SceneHierarchyBVH_RTIOW::SceneHierarchyBVH_RTIOW() :
			m_RootNode(nullptr)
		{
		}

		SceneHierarchyBVH_RTIOW::~SceneHierarchyBVH_RTIOW()
		{
			Clear();
		}

		void SceneHierarchyBVH_RTIOW::Clear()
		{
			if (m_RootNode != nullptr)
			{
				delete m_RootNode;
				m_RootNode = nullptr;
			}
		}

		void SceneHierarchyBVH_RTIOW::Build(SceneObject** sceneObjects, size_t sceneObjectsCount)
		{
			m_RootNode = new SceneObjectBVHNode(sceneObjects, 0, sceneObjectsCount);
		}

		bool SceneHierarchyBVH_RTIOW::FindIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInf)
		{
			if (m_RootNode == nullptr) return false;
			return m_RootNode->CheckIntersection(rayInfo, hitInf);
		}

		SceneObjectBVHNode::SceneObjectBVHNode(SceneObject** sceneObjects, size_t start, size_t end) :
			m_LeftChild(nullptr),
			m_RightChild(nullptr),
			m_IsFinalNode(false)
		{
			const size_t rangeSize = end - start;
			if (rangeSize > 2)
			{
				size_t splitAxis = 0;
				//Calculate aabb for this node
				// - aproach with reconstructing local aabb on every node
				//   very slow but currently i have no idea how to speed up this
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
				;
				m_AABB = localAABB;
				m_LeftChild = new SceneObjectBVHNode(sceneObjects, start, mid);// left
				m_RightChild = new SceneObjectBVHNode(sceneObjects, mid, end); // right
				m_IsFinalNode = false;
			}
			else if (rangeSize == 2)
			{
				m_AABB = AABB::SurroundingAABB(sceneObjects[start]->getAABB(), sceneObjects[start + 1]->getAABB());
				m_LeftChild = sceneObjects[start];
				m_RightChild = sceneObjects[start + 1];
				m_IsFinalNode = true;
			}
			else if (rangeSize == 1)
			{
				m_AABB = sceneObjects[start]->getAABB();
				m_LeftChild = sceneObjects[start];
				m_RightChild = sceneObjects[start];
				m_IsFinalNode = true;
			}
		}

		SceneObjectBVHNode::~SceneObjectBVHNode()
		{
			if (!m_IsFinalNode)
			{
				delete m_LeftChild;
				delete m_RightChild;
			}
		}

		bool SceneObjectBVHNode::CheckIntersection(RayInfo& rayInfo, SurfaceHitInfo& hitInfo)
		{
			if (m_AABB.CheckIntersection(rayInfo))
			{
				bool hitLeft = m_LeftChild->CheckIntersection(rayInfo, hitInfo);
				bool hitRight = m_RightChild->CheckIntersection(rayInfo, hitInfo);
				return hitLeft || hitRight;
			}
			return false;
		}
		AABB const& SceneObjectBVHNode::getAABB() const
		{
			return m_AABB;
		}
	}
}