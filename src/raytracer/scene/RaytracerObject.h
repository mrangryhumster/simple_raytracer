#pragma once

#include "RaytracerTypes.h"

namespace Raytracer
{
	struct RaytracerObject
	{
		//Creates simple default object
		inline static std::shared_ptr<RaytracerObject> Default(uint64_t id = 0)
		{
			auto defaultObject = std::shared_ptr<RaytracerObject>(new RaytracerObject(id), Destroy);
			defaultObject->m_MeshId = 0;
			defaultObject->m_MaterialId = 0;
			defaultObject->m_Position = vec3f(0.0f);
			return defaultObject;
		}

		inline static std::shared_ptr<RaytracerObject> Create(uint64_t id)
		{
			return std::shared_ptr<RaytracerObject>(new RaytracerObject(id), Destroy);
		}

		const uint64_t m_Id;
		size_t m_MeshId;
		size_t m_MaterialId;
		vec3f m_Position;

	protected: //Prohibit default constructor
	//Object should be created via static method "Create"
		RaytracerObject(uint64_t id) : m_Id(id) {};

		//Object deletion should be handled by shared_ptr exclusevly
		inline static void Destroy(RaytracerObject* obj) { delete obj; }
	};
}