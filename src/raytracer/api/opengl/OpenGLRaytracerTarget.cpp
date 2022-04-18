#include "OpenGLRaytracerTarget.h"

namespace API
{
	namespace OpenGL
	{
		OpenGLRaytracerTarget::OpenGLRaytracerTarget() :
			m_glTexture(0),
			m_PixelsData(nullptr),
			m_Width(0),
			m_Height(0)
		{}

		OpenGLRaytracerTarget::~OpenGLRaytracerTarget()
		{
			Destroy();
		}

		void OpenGLRaytracerTarget::Initialize()
		{
			glGenTextures(1, &m_glTexture);
			this->Resize(4, 4); //dummy texture
		}

		void OpenGLRaytracerTarget::Destroy()
		{
			if (m_glTexture != 0)
			{
				glDeleteTextures(1, &m_glTexture);
				m_glTexture = 0;
			}

			if (m_PixelsData != nullptr)
			{
				delete[] m_PixelsData;
				m_PixelsDataSize = 0;
			}
		}

		void OpenGLRaytracerTarget::Sync(bool trySync)
		{
			bool isObjectLocked = false;
			if (trySync)
			{
				isObjectLocked = m_LockMutex.try_lock();
			}
			else
			{
				m_LockMutex.lock();
				isObjectLocked = true;
			}

			if (isObjectLocked)
			{
				//Create PBO buffer
				GLuint intermediatePixelBufferObject;
				glGenBuffers(1, &intermediatePixelBufferObject);
				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, intermediatePixelBufferObject);
				glBufferData(
					GL_PIXEL_UNPACK_BUFFER,
					m_PixelsDataSize,
					m_PixelsData,
					GL_STREAM_DRAW
				);

				//Update texture from pbo
				glBindTexture(GL_TEXTURE_2D, m_glTexture);
				glTexSubImage2D(
					GL_TEXTURE_2D,
					0,
					0,
					0,
					m_Width,
					m_Height,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					nullptr);

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
				glDeleteBuffers(1, &intermediatePixelBufferObject);

				m_LockMutex.unlock();
			}
		}

		void OpenGLRaytracerTarget::Resize(uint32_t width, uint32_t height)
		{
			if (m_Width != width || m_Height != height)
			{
				m_LockMutex.lock(); //Lock object

				if (m_PixelsData != nullptr)
					delete[] m_PixelsData;

				m_PixelsDataSize = width * height * 3;
				m_PixelsData = new uint8_t[m_PixelsDataSize];

				m_Width = width;
				m_Height = height;
				glBindTexture(GL_TEXTURE_2D, m_glTexture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGB,
					m_Width,
					m_Height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					nullptr);
					
				m_LockMutex.unlock(); //Unlock object
			}
		}

		void OpenGLRaytracerTarget::Clear(color3f clearColor)
		{
			memset(m_PixelsData, 0, m_PixelsDataSize);
		}

		bool OpenGLRaytracerTarget::Lock(bool tryLock)
		{
			if (tryLock)
			{
				return m_LockMutex.try_lock();
			}
			else
			{
				m_LockMutex.lock();
				return true;
			}
		}

		void OpenGLRaytracerTarget::Unlock()
		{
			m_LockMutex.unlock();
		}

		uint32_t OpenGLRaytracerTarget::getWidth()
		{
			return m_Width;
		}

		uint32_t OpenGLRaytracerTarget::getHeight()
		{
			return m_Height;
		}

		void* OpenGLRaytracerTarget::getAPIObject()
		{
			return reinterpret_cast<void*>(m_glTexture);
		}
		size_t OpenGLRaytracerTarget::getAPIObjectSize()
		{
			return sizeof(GLuint);
		}

		void* OpenGLRaytracerTarget::getData()
		{
			return m_PixelsData;
		}
		size_t OpenGLRaytracerTarget::getDataSize()
		{
			return m_PixelsDataSize;
		}

	}
}