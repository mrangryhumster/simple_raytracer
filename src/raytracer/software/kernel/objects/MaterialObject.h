#pragma once

#include "SoftwareKernel.h"

//MACRO BEGIN
#define __RT_MATERIAL_GENERATE_FIELD(field,type,textureAccessor)		\
bool m_Is##field##Texture;												\
union { type Value;TextureObject* Texture;} m_##field##;				\
inline type get##field##(vec2f uv) const {								\
if(this->m_Is##field##Texture)											\
	return this->m_##field##.Texture->textureAccessor(uv.x,uv.y);		\
return this->m_##field##.Value;											\
}
//MACRO END


namespace Raytracer
{
	namespace Software
	{
		class MaterialObject
		{
		public:
			//----------------------------------------------------------------
			static inline RayInfo GetScatterRay(RayInfo const& rayInfo, SurfaceHitInfo const& hitInfo)
			{
				MaterialObject const * const material = hitInfo.m_Material;

				vec3 unitDirection = normalize(rayInfo.m_Direction);
				float refractionRatio = (hitInfo.m_IsFrontFace) ? (1.0f / material->getIOR(hitInfo.m_UV)) : material->getIOR(hitInfo.m_UV);
				float cosTheta = nmin<float>(dot(-unitDirection, hitInfo.m_Normal), 1.0);
				float sinTheta = nsqrt<float>(1.0f - cosTheta * cosTheta);

				vec3 objNormal = hitInfo.m_Normal + (random_hemisphere_vector(hitInfo.m_Normal) * material->getRoughness(hitInfo.m_UV));
				
				vec3 dirVector;
				bool cannot_refract = refractionRatio * sinTheta > 1.0;
				if (cannot_refract || reflectance(cosTheta, refractionRatio) > random_uniform_float() || material->getTransmission(hitInfo.m_UV) < random_uniform_float())
					dirVector = reflect(unitDirection, objNormal);
				else
					dirVector = refract(unitDirection, objNormal, refractionRatio);

				return RayInfo(hitInfo.m_Position + (dirVector * RAY_MIN_DISTANCE), normalize(dirVector));
			}

			//----------------------------------------------------------------
			static inline color3f ColorBlending(color3f const& subRayColor, SurfaceHitInfo const& hitInfo)
			{
				return  
					hitInfo.m_Material->getEmissionColor(hitInfo.m_UV) * hitInfo.m_Material->getEmissionStrength(hitInfo.m_UV) +
					subRayColor * hitInfo.m_Material->getAlbedo(hitInfo.m_UV);
			}
			//----------------------------------------------------------------
			__RT_MATERIAL_GENERATE_FIELD(Albedo, color3f, getVec3f);
			__RT_MATERIAL_GENERATE_FIELD(Metallic, float, getVec1f);
			__RT_MATERIAL_GENERATE_FIELD(Roughness, float, getVec1f);
			__RT_MATERIAL_GENERATE_FIELD(IOR, float, getVec1f);
			__RT_MATERIAL_GENERATE_FIELD(Transmission, float, getVec1f);
			__RT_MATERIAL_GENERATE_FIELD(EmissionStrength, float, getVec1f);
			__RT_MATERIAL_GENERATE_FIELD(EmissionColor, color3f, getVec3f);

		};
	}
}