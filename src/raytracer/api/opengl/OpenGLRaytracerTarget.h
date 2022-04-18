#pragma once

#include <mutex>

#include "OpenGLUtils.h"
#include "RaytracerTargetInterface.h"

namespace API
{
	namespace OpenGL
	{
		class OpenGLRaytracerTarget : public Raytracer::RaytracerTargetInterface
		{
		public:
			OpenGLRaytracerTarget();
			~OpenGLRaytracerTarget() override;

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
			std::mutex	m_LockMutex;
			GLuint		m_glTexture;
			
			uint8_t*	m_PixelsData;
			size_t		m_PixelsDataSize;
			GLsizei     m_Width;
			GLsizei     m_Height;
		};
	}
}