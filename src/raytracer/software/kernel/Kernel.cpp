#include "Kernel.h"

namespace Raytracer
{
	namespace Software
	{
		void Kernel::Raytrace(KernelConfig kernelCfg, TargetConfig targetCfg, RegionConfig regionCfg, SceneConfig sceneCfg)
		{
			//****************************************************************
			const uint32_t ypixel_max = (regionCfg.m_StartY + regionCfg.m_Height);
			const uint32_t xpixel_max = (regionCfg.m_StartX + regionCfg.m_Width);

			for (uint32_t ypixel = regionCfg.m_StartY; ypixel < ypixel_max; ypixel++)
			{
				color3u* pixelRow = (color3u*)targetCfg.m_Framebuffer.getData(regionCfg.m_StartX, ypixel);
				for (uint32_t xpixel = regionCfg.m_StartX; xpixel < xpixel_max; xpixel++)
				{
					//-------------------------------------------------------
					color3f pixelColor(0, 0, 0);
					float tWidth = static_cast<float>(targetCfg.m_Framebuffer.getWidth() - 1.0f);
					float tHeight = static_cast<float>(targetCfg.m_Framebuffer.getHeight() - 1.0f);
					//-------------------------------------------------------
					//Samples > 1 = Multisampling
					for (uint32_t sample = 0; sample < regionCfg.m_Samples - 1; sample++)
					{
						//Check for termination flag every 128 samples
						if (sample % 128 && kernelCfg.m_TerminateFlag->load()) return;

						const float u = (static_cast<float>(xpixel) + random_uniform_float()) / tWidth;
						const float v = (static_cast<float>(ypixel) + random_uniform_float()) / tHeight;
						RayInfo ray = sceneCfg.m_SceneCamera.CastRay(u, v);
						pixelColor += Kernel::Trace(kernelCfg.m_RayMaxBounces + 1, ray, sceneCfg);
					}

					//One ray always shots in the center of pixel (rest of the code same as above)
					{
						
						const float u = (static_cast<float>(xpixel) + 0.5f) / tWidth;
						const float v = (static_cast<float>(ypixel) + 0.5f) / tHeight;
						RayInfo ray = sceneCfg.m_SceneCamera.CastRay(u, v);
						pixelColor += Kernel::Trace(kernelCfg.m_RayMaxBounces + 1, ray, sceneCfg);
					}
					//-------------------------------------------------------
					*(pixelRow++) = ConvertColor(pixelColor, regionCfg.m_Samples, 0.0f);
				}
			}
			//****************************************************************
		}

		color3f Kernel::Trace(
			uint32_t depth,
			RayInfo& rayInfo,
			const SceneConfig& scene)
		{
			if (depth == 0) return color3f(0.0f, 0.0f, 0.0f);


			SurfaceHitInfo	hitInfo;
			if (scene.m_SceneHierarchy->FindIntersection(rayInfo, hitInfo))
			{
				RayInfo scatterRay = MaterialObject::GetScatterRay(rayInfo, hitInfo);
				color3f colorAccumulator = Trace(depth - 1, scatterRay, scene);

				return MaterialObject::ColorBlending(colorAccumulator, hitInfo);
				//return (colorAccumulator = color3f(hitInfo.m_Normal.x, hitInfo.m_Normal.y, hitInfo.m_Normal.z));
			}

			//return ColorGradient((rayInfo.m_Direction.y + 1.0f)*0.5f, Gradient::Skybox_Data, Gradient::Skybox_Curve, Gradient::Skybox_Size);
			//return ColorGradient((rayInfo.m_Direction.y + 1.0f) * 0.5f, color3f(1.f, 1.f, 1.f), color3f(0.5f, 0.7f, 1.0f));
			//return color3f(1.0f,1.0f,1.0f);
			return color3f(0.0f, 0.0f, 0.0f);
		}
	}
}