#pragma once

#include "RaytracerTypes.h"

namespace Raytracer
{
	enum class RaytracerMeshType
	{
		Empty,
		Sphere
	};

	struct RaytracerMesh
	{
		const uint64_t m_Id;
		const RaytracerMeshType m_Type;

		protected: //Prohibit default constructor
			//Object should be created via derived class
			RaytracerMesh(uint64_t id, RaytracerMeshType meshType) : m_Id(id), m_Type(meshType) {};
	};

	struct RaytracerMeshEmpty : RaytracerMesh
	{
		inline static std::shared_ptr<RaytracerMeshEmpty> Create(uint64_t id)
		{
			return std::shared_ptr<RaytracerMeshEmpty>(new RaytracerMeshEmpty(id, RaytracerMeshType::Empty), Destroy);
		}

	private: //Prohibit default constructor
		//Object should be created via static method "Create"
		RaytracerMeshEmpty(uint64_t id, RaytracerMeshType meshType) : RaytracerMesh(id, meshType) {};

		//Object deletion should be handled by shared_ptr exclusevly
		inline static void Destroy(RaytracerMeshEmpty* obj) { delete obj; };
	};

	struct RaytracerMeshSphere : RaytracerMesh
	{
		inline static std::shared_ptr<RaytracerMeshSphere> Create(uint64_t id)
		{
			return std::shared_ptr<RaytracerMeshSphere>(new RaytracerMeshSphere(id, RaytracerMeshType::Sphere), Destroy);
		}

		float m_Radius;
	private: //Prohibit default constructor
		//Object should be created via static method "Create"
		RaytracerMeshSphere(uint64_t id, RaytracerMeshType meshType) : RaytracerMesh(id, meshType) {};

		//Object deletion should be handled by shared_ptr exclusevly
		inline static void Destroy(RaytracerMeshSphere* obj) { delete obj; };
	};
}