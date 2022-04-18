#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software	
	{
		inline double reflectance(float cosine, float ref_idx) 
		{
			// Use Schlick's approximation for reflectance.
			auto r0 = (1 - ref_idx) / (1 + ref_idx);
			r0 = r0 * r0;
			return r0 + (1 - r0) * pow((1 - cosine), 5);
		}
	}
}