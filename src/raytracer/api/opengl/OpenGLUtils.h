#pragma once

#include "GL/gl3w.h"
#include "RaytracerTypes.h"

namespace API
{
	namespace OpenGL
	{
		//----------------------------------------------------------------
		inline GLint getGlInternalPixelFormat(Raytracer::TexturePixelFormat format)
		{
			switch (format)
			{
			case Raytracer::TexturePixelFormat::RGB:
				return GL_RGB;
			case Raytracer::TexturePixelFormat::RGBA:
				return GL_RGBA;
			case Raytracer::TexturePixelFormat::BGR:
				return GL_BGR;
			case Raytracer::TexturePixelFormat::BGRA:
				return GL_BGRA;
			case Raytracer::TexturePixelFormat::DEPTH:
				return GL_DEPTH_COMPONENT;

			default:
				return GL_RGBA;
			}
		}
		//----------------------------------------------------------------
		//Same as glInternalPixelFormat for now
		inline GLenum getGlTexturePixelDataFormat(Raytracer::TexturePixelFormat format)
		{
			return getGlInternalPixelFormat(format);
		}
		//----------------------------------------------------------------
		inline GLenum getGlTexturePixelDataType(Raytracer::TexturePixelType type)
		{
			switch (type)
			{
			case Raytracer::TexturePixelType::UINT8:
				return  GL_UNSIGNED_BYTE;
			case Raytracer::TexturePixelType::UINT32:
				return  GL_UNSIGNED_INT;
			case Raytracer::TexturePixelType::FLOAT:
				return	GL_FLOAT;
			default:
				return  GL_UNSIGNED_BYTE;
			}
		}
	}
}