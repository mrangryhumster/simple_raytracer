#pragma once

#include "SoftwareKernel.h"

namespace Raytracer 
{
	namespace Software
	{
		using namespace nova::math;

		inline float random_uniform_float()
		{
			static std::mt19937 generator(0);
			static thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
			return distribution(generator);
		}

		inline float random_uniform_float(float min,float max)
		{
			static std::mt19937 generator(0);
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(generator);
		}
		

		//Omg... what have i done...

		inline nvec3<float> random_color()
		{
			static thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
			static std::mt19937 generator(0);
			return nvec3<float>(
				distribution(generator),
				distribution(generator),
				distribution(generator));
		}

		inline vec3 random_vector_2d() 
		{
			static thread_local std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
			static std::mt19937 generator(0);

			while (true) {
				vec3 rndVector(
					distribution(generator), 
					distribution(generator),
					0);
				if (length_squared(rndVector) >= 1.0f) continue;
				return rndVector;
			}
		}

		inline vec3 random_vector_3d()
		{
			static thread_local std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
			static std::mt19937 generator(0);

			while (true)
			{
				vec3 rndVector(
					distribution(generator),
					distribution(generator),
					distribution(generator));
				if (length_squared(rndVector) >= 1.0f) continue;
				return rndVector;
			}
		}

		inline vec3 random_hemisphere_vector(vec3 const& normal)
		{
			vec3 rndVector = random_vector_3d();
			if (dot(rndVector, normal) > 0.0f)
				return (rndVector);
			else
				return -(rndVector);
		}
	}
}