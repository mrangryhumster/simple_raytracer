#pragma once

#include "RaytracerTypes.h"

#include <memory>

namespace Raytracer
{
	struct RaytracerTexture
	{
		//Create default texture
		inline static std::shared_ptr<RaytracerTexture> Default(uint64_t id = 0)
		{
			auto defaultTexture = std::shared_ptr<RaytracerTexture>(new RaytracerTexture(id), Destroy);
			defaultTexture->m_Width = 4;
			defaultTexture->m_Height = 4;
			defaultTexture->m_Data = new uint8_t[48];
			defaultTexture->m_DataSize = 48;
			return defaultTexture;
		}

		//Create texture struct and assign data to it
		//WARNING! Assigned data will be deleted with object when disposed
		inline static std::shared_ptr<RaytracerTexture> Create(uint64_t id)
		{
			return std::shared_ptr<RaytracerTexture>(new RaytracerTexture(id), Destroy);
		}

		const uint64_t m_Id;
		uint32_t m_Width;
		uint32_t m_Height;
		uint8_t  m_Components;
		uint8_t* m_Data;
		size_t   m_DataSize;

	protected: //Prohibit default constructor
		//Object should be created via static method "Create"
		RaytracerTexture(uint64_t id) : m_Id(id), m_Width(0), m_Height(0), m_Data(nullptr), m_DataSize(0) {};

		//Object deletion should be handled by shared_ptr exclusevly
		inline static void Destroy(RaytracerTexture* obj)
		{
			if (obj->m_Data != nullptr)	
				delete[] obj->m_Data;
			delete obj;
		}

	};
}