#pragma once

#include <cstdio>
#include "RaytracerTargetInterface.h"

namespace API
{
	namespace Null
	{
		class NullRaytracerTarget : public Raytracer::RaytracerTargetInterface
		{
		public:
			NullRaytracerTarget();
			~NullRaytracerTarget() override;

			void Initialize() override;
			void Destroy() override;

			void Sync(bool trySync) override;
			void Resize(uint32_t width, uint32_t height) override;
			void Clear(color3f clearColor) override;

			bool Lock(bool tryLock) override;
			void Unlock() override;

			uint32_t getWidth() override;
			uint32_t getHeight() override;

			void* getAPIObject() override;
			size_t getAPIObjectSize() override;

			void* getData() override;
			size_t getDataSize() override;

		private:
			uint8_t*	 m_Data;
			size_t		 m_Size;
			uint32_t     m_Width;
			uint32_t     m_Height;
		};
	}
}