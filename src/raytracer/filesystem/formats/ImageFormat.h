#pragma once

#include <cstdint>
#include <string>

#include "RaytracerLogger.h"
#include "RaytracerTexture.h"

namespace Raytracer
{
	namespace FileSystem
	{
		enum class ImageFormatType
		{
			ANY,
			PNG,
			JPEG,
			TGA,
			BMP
		};

		class ImageFormat
		{
		public:
			

			// --- Direct functions to save/load data ---

			//Direct pixels save
			static bool Export(std::string file_path, ImageFormatType format, uint32_t width, uint32_t height, void* data_ptr, size_t data_size);
			static bool Export(void** exported_data, size_t& exported_size, ImageFormatType format, uint32_t width, uint32_t height, void* data_ptr, size_t data_size);
			//Direct pixels load
			static bool Import(std::string file_path, uint32_t& width, uint32_t& height, uint8_t& comp, uint8_t*& data_ptr, size_t& data_size);
			static bool Import(void** import_data, size_t& import_size, uint32_t& width, uint32_t& height, uint8_t& comp, uint8_t*& data_ptr, size_t& data_size);
		};
	}
}