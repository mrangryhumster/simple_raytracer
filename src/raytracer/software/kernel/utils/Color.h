#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		//-------------------------------------------------------------------------------------------------------------------------------
		namespace Gradient
		{
			static const size_t Heatmap_Size = 7;
			static const ncolor3<float> HeatMap_Data[Heatmap_Size]{
			ncolor3<float>(0,0,0),
			ncolor3<float>(0,0,1.0f),
			ncolor3<float>(0,1.0f,1.0f),
			ncolor3<float>(0,1.0f,0),
			ncolor3<float>(1.0f,1.0f,0),
			ncolor3<float>(1.0f,0,0),
			ncolor3<float>(1.0f,1.0f,1.0f)
			};

			static const size_t Skybox_Size = 5;
			static const ncolor3<float> Skybox_Data[Heatmap_Size]{
			ncolor3<float>(0.34f,0.32f,0.30f),
			ncolor3<float>(0.41f,0.38f,0.36f),
			ncolor3<float>(0.71f,0.96f,1.00f),
			ncolor3<float>(0.44f,0.70f,1.00f),
			ncolor3<float>(0.37f,0.64f,1.00f)
			};
			static const float Skybox_Curve[Skybox_Size]{
				0.0f,0.45f,0.5f,0.7f,1.0f
			};
		};
		//-------------------------------------------------------------------------------------------------------------------------------
		inline ncolor3<float> ColorGradient(float value, ncolor3<float> const& min_color, ncolor3<float> const& max_color)
		{
			if (value > 1.0f) return max_color;
			else if (value < 0.0f)  return min_color;

			return min_color * (1.0f - value) + max_color * value;
		}
		//-------------------------------------------------------------------------------------------------------------------------------
		inline ncolor3<float> ColorGradient(float value, const ncolor3<float>* gradient_data, size_t gradient_size)
		{
			if (value > 1.0f) return gradient_data[gradient_size - 1];
			else if (value < 0.0f)  return gradient_data[0];

			value *= gradient_size - 1;
			size_t ramp_index_1 = static_cast<size_t>(floor(value));
			size_t ramp_index_2 = static_cast<size_t>(ramp_index_1 + 1);
			float frac = value - (float)ramp_index_1;

			return ncolor3<float>(
				static_cast<float>((gradient_data[ramp_index_2].r - gradient_data[ramp_index_1].r) * frac + gradient_data[ramp_index_1].r),
				static_cast<float>((gradient_data[ramp_index_2].g - gradient_data[ramp_index_1].g) * frac + gradient_data[ramp_index_1].g),
				static_cast<float>((gradient_data[ramp_index_2].b - gradient_data[ramp_index_1].b) * frac + gradient_data[ramp_index_1].b));
		}
		//-------------------------------------------------------------------------------------------------------------------------------
		inline ncolor3<float> ColorGradient(float value, const ncolor3<float>* gradient_data, const float* gradient_curve, size_t gradient_size)
		{
			if (value > 1.0f) return gradient_data[gradient_size - 1];
			else if (value < 0.0f)  return gradient_data[0];

			size_t ramp_index_1 = 0;
			size_t ramp_index_2 = 0;

			while (value > gradient_curve[ramp_index_2])
				ramp_index_2++;
			ramp_index_1 = ramp_index_2 - 1;

			if (ramp_index_1 >= gradient_size || ramp_index_2 >= gradient_size)
				return gradient_data[0];

			float frac = map_value<float, float>(value, gradient_curve[ramp_index_1], gradient_curve[ramp_index_2]);

			return ncolor3<float>(
				static_cast<float>((gradient_data[ramp_index_2].r - gradient_data[ramp_index_1].r) * frac + gradient_data[ramp_index_1].r),
				static_cast<float>((gradient_data[ramp_index_2].g - gradient_data[ramp_index_1].g) * frac + gradient_data[ramp_index_1].g),
				static_cast<float>((gradient_data[ramp_index_2].b - gradient_data[ramp_index_1].b) * frac + gradient_data[ramp_index_1].b));
		}
		//-------------------------------------------------------------------------------------------------------------------------------
		inline ncolor3<uint8_t> ConvertColor(const ncolor3<float>& cf32, uint32_t samples, float gamma)
		{
			auto r = cf32.r;
			auto g = cf32.g;
			auto b = cf32.b;

			if (r != r) r = 0.0f;
			if (g != g) g = 0.0f;
			if (b != b) b = 0.0f;

			auto scale = 1.0 / samples;
			r = sqrt(scale * r);
			g = sqrt(scale * g);
			b = sqrt(scale * b);

			if (r > 0.999f) r = 0.999f;
			else if (r < 0.0f) r = 0.0f;
			if (g > 0.999f) g = 0.999f;
			else if (g < 0.0f) g = 0.0f;
			if (b > 0.999f) b = 0.999f;
			else if (b < 0.0f) b = 0.0f;

			return ncolor3<uint8_t>(255 * r,255 * g,255 * b);
		}
		//-------------------------------------------------------------------------------------------------------------------------------
	}
}