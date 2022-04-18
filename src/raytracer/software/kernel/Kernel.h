#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class Kernel
		{
		public:

			static void Raytrace(
				KernelConfig kernelCfg,
				TargetConfig targetCfg,
				RegionConfig regionCfg,
				SceneConfig sceneCfg
			);

			static color3f Trace(
				uint32_t depth,
				RayInfo& ray,
				KernelInternals& internals
			);

		};
	}
}