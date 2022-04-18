#pragma once

#include "SoftwareKernel.h"


namespace Raytracer
{
	namespace Software
	{
		class TextureObject
		{
		public:
			TextureObject() :
				m_Width(4), 
				m_Height(4), 
				m_Data(new uint8_t[48]), 
				m_DataSize(48) ,
				m_IsReferenceMode(false)
			{};

			~TextureObject()
			{
				Destroy();
			}

			inline void Initialize(uint32_t width, uint32_t height, uint8_t components, void* data, size_t size, bool referenceMode = false)
			{
				//Clear previous data
				Destroy();

				//Safe check, if any of these true - override parameters with '4x4 default texture'
				if (width == 0 || height == 0 || data == nullptr || size == 0)
				{
					width = 4;
					height = 4;
					components = 3;
					data = new uint8_t[48];
					size = 48;
					referenceMode = false;
				}

				m_Width = width;
				m_Height = height;
				m_Components = components;
				m_Area = m_Width * m_Height;
				m_DataSize = size;
				if (referenceMode == false)
				{
					m_Data = new uint8_t[size];
					std::memcpy(m_Data, data, size);
					m_IsReferenceMode = false;
				}
				else
				{
					m_Data = reinterpret_cast<uint8_t*>(data);
					m_IsReferenceMode = true;
				}
			}

			inline void Destroy()
			{
				if (m_IsReferenceMode == false && m_Data != nullptr)
				{
					delete[] m_Data;
					m_Data = nullptr;
				}
			}

			inline float getVec1f(float u, float v) const
			{
				uint8_t* offset = getPointerFromUV(u, v);
				const float scale_correction = 1.0f / 255.0f;
				return offset[0] * scale_correction;
			}

			inline nvec<2,float> getVec2f(float u, float v) const
			{
				uint8_t* offset = getPointerFromUV(u, v);
				const float scale_correction = 1.0f / 255.0f;
				return nvec<2, float>(offset[0], offset[1]) * scale_correction;
			}

			inline nvec<3, float> getVec3f(float u, float v) const
			{
				uint8_t* offset = getPointerFromUV(u, v);
				const float scale_correction = 1.0f / 255.0f;
				return nvec<3, float>(offset[0], offset[1], offset[2]) * scale_correction;
			}

			inline uint8_t* getData()
			{
				return m_Data;
			}

			inline uint8_t* getData(float u, float v)
			{
				return m_Data + ((static_cast<size_t>(v * m_Area)) + (static_cast<size_t>(u * m_Width))) * m_Components;
			}

			inline uint8_t* getData(uint32_t x, uint32_t y)
			{
				return m_Data + static_cast<size_t>((y * m_Width) + x) * m_Components;
			}

			inline const size_t getDataSize() const
			{
				return m_DataSize;
			}

			inline const uint32_t getWidth() const
			{
				return m_Width;
			}

			inline const uint32_t getHeight() const
			{
				return m_Height;
			}

			inline const uint32_t getArea() const
			{
				return m_Area;
			}


		private:
			uint32_t m_Width;
			uint32_t m_Height;
			uint8_t	 m_Components;
			uint32_t m_Area;
			uint8_t* m_Data;
			size_t   m_DataSize;
			bool	 m_IsReferenceMode;

			inline uint8_t* getPointerFromUV(float u, float v) const
			{
				while (u >= 1.0f) u -= 1.0f;
				while (u < 0) u += 1.0f;
				while (v >= 1.0f) v -= 1.0f;
				while (v < 0) v += 1.0f;
				
				v = 1.0f - v;

				size_t x = static_cast<size_t>(u * m_Width);
				size_t y = static_cast<size_t>(v * m_Height);

				if (x >= m_Width) x = m_Width - 1;
				if (y >= m_Height) y = m_Height - 1;

				return m_Data + (y * m_Width + x) * 4;
			}
		};
	}
}