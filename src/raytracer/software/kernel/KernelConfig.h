#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct KernelConfig
		{
			uint32_t m_RayMaxBounces;

			std::atomic_bool* m_TerminateFlag = nullptr;
		};
	}
}