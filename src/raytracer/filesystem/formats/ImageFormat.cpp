#include "ImageFormat.h"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Raytracer
{
	namespace FileSystem
	{
		bool ImageFormat::Export(std::string file_path, ImageFormatType format, uint32_t width, uint32_t height, void* data_ptr, size_t data_size)
		{
			int result = 0;
			switch (format)
			{
			case ImageFormatType::PNG:
				result = stbi_write_png(file_path.c_str(), width, height, 3, data_ptr, width * 3);
				break;
			case ImageFormatType::JPEG:
				result = stbi_write_jpg(file_path.c_str(), width, height, 3, data_ptr, 1);
				break;
			case ImageFormatType::TGA:
				result = stbi_write_tga(file_path.c_str(), width, height, 3, data_ptr);
				break;
			case ImageFormatType::BMP:
				result = stbi_write_bmp(file_path.c_str(), width, height, 3, data_ptr);
				break;
			case ImageFormatType::ANY:
				result = stbi_write_png((file_path + ".png").c_str(), width, height, 3, data_ptr, width * 3);
				break;

			}

			return result != 0;
		}

		bool ImageFormat::Import(std::string file_path, uint32_t& width, uint32_t& height, uint8_t& comp, uint8_t*& data_ptr, size_t& data_size)
		{
			int w,h,c;
			stbi_uc* stbImageData = stbi_load(file_path.c_str(), &w, &h, &c, STBI_rgb_alpha); //Force to load as RGBA
			if (stbImageData == nullptr)
			{
				LOG_ERROR("Unable to load image \"%s\", stbi error : %s\n", file_path.c_str(), stbi_failure_reason());
				return false;
			}
			
			width = w;
			height = h;
			comp = 4;
			data_ptr = stbImageData;
			data_size = w * h * 4;
			
			return true;
		}
	}
}