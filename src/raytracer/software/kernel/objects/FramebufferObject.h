#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		class FramebufferObject
		{
		public:

			inline void Link(uint32_t width, uint32_t height, void* data)
			{
				m_Width = width;
				m_Height = height;
				m_Data = reinterpret_cast<uint8_t*>(data);
			}

			inline uint8_t* getData(uint32_t x, uint32_t y)
			{
				return m_Data + static_cast<size_t>((y * m_Width) + x) * sizeof(color3u);
			}

			inline const uint32_t getWidth() const
			{
				return m_Width;
			}

			inline const uint32_t getHeight() const
			{
				return m_Height;
			}


		private:
			uint32_t m_Width;
			uint32_t m_Height;
			uint8_t* m_Data;
		};
	}
}