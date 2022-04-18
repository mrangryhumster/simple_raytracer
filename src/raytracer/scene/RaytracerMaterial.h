#pragma once

#include "RaytracerTypes.h"
#include "RaytracerTexture.h"

namespace Raytracer
{
	struct RaytracerMaterial
	{
		//Creates dafault material
		inline static std::shared_ptr<RaytracerMaterial> Default(uint64_t id = 0)
		{
			auto newMaterial = std::shared_ptr<RaytracerMaterial>(new RaytracerMaterial(id), Destroy);
			newMaterial->m_IsAlbedoTexture = false;
			newMaterial->m_Albedo.Value = color3f(1.0f);
			newMaterial->m_IsMetallicTexture = false;
			newMaterial->m_Metallic.Value = 0.f;
			newMaterial->m_IsRoughnessTexture = false;
			newMaterial->m_Roughness.Value = 1.f;
			newMaterial->m_IsIORTexture = false;
			newMaterial->m_IOR.Value = 0.f;
			newMaterial->m_IsTransmissionTexture = false;
			newMaterial->m_Transmission.Value = 0.f;

			newMaterial->m_IsEmissionStrengthTexture = false;
			newMaterial->m_EmissionStrength.Value = 0.f;
			newMaterial->m_IsEmissionColorTexture = false;
			newMaterial->m_EmissionColor.Value = color3f(0.0f);

			return newMaterial;
		}

		inline static std::shared_ptr<RaytracerMaterial> Create(uint64_t id)
		{
			return std::shared_ptr<RaytracerMaterial>(new RaytracerMaterial(id), Destroy);
		}
		//uint64_t Texture;
		const uint64_t m_Id;

		
		bool m_IsAlbedoTexture;
		union {
			color3f Value;
			uint64_t TextureId;
		} m_Albedo;

		
		bool m_IsMetallicTexture;
		union {
			float	Value;
			uint64_t TextureId;
		} m_Metallic;

		bool m_IsRoughnessTexture;
		union {
			float	Value;
			uint64_t TextureId;
		} m_Roughness;

		bool m_IsIORTexture;
		union {
			float	Value;
			uint64_t TextureId;
		} m_IOR;

		bool m_IsTransmissionTexture;
		union  {
			float	Value;
			uint64_t TextureId;
		} m_Transmission;

		bool m_IsEmissionStrengthTexture;
		union {
			float	Value;
			uint64_t TextureId;
		} m_EmissionStrength;

		bool m_IsEmissionColorTexture;
		union {
			color3f	Value;
			uint64_t TextureId;
		} m_EmissionColor;

	protected: //Prohibit default constructor
		//Object should be created via static method "Create"
		RaytracerMaterial(uint64_t id) : m_Id(id) {};

		//Object deletion should be handled by shared_ptr exclusevly
		inline static void Destroy(RaytracerMaterial* obj) { delete obj; }
	};
}