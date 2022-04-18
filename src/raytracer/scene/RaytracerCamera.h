#pragma once

#include "RaytracerTypes.h"

namespace Raytracer
{
	struct RaytracerCamera
	{
		//Creates simple dafault camera
		inline static std::shared_ptr<RaytracerCamera> Default(uint64_t id = 0)
		{
			auto defaultCamera = std::shared_ptr<RaytracerCamera>(new RaytracerCamera(id), Destroy);
			defaultCamera->m_Position = vec3(0, 0, 0);
			defaultCamera->m_Target = vec3(0, 0, 1);
			defaultCamera->m_Up = vec3(0, 1, 0);
			defaultCamera->m_FOV = 60.f;
			defaultCamera->m_UseDepthOfDield = false;
			return defaultCamera;
		}

		inline static std::shared_ptr<RaytracerCamera> Create(uint64_t id)
		{
			return std::shared_ptr<RaytracerCamera>(new RaytracerCamera(id), Destroy);
		}

		const uint64_t m_Id;

		vec3 m_Position;
		vec3 m_Target;
		vec3 m_Up;

		float m_FOV;
		float m_Aperture;
		float m_FocusDist;
		float m_Aspect;

		bool m_UseDepthOfDield;

	protected: //Prohibit default constructor
		//Object should be created via static method "Create"
		RaytracerCamera(uint64_t id) : m_Id(id) {};

		//Object deletion should be handled by shared_ptr exclusevly
		inline static void Destroy(RaytracerCamera* obj) { delete obj; }

	};
}