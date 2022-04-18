#pragma once

#include <cstdint>

#define _DEBUG
#if defined(_DEBUG)
#	define LOG_LDEBUG(MSG,...) Raytracer::Logger::Write(0xFF,MSG,##__VA_ARGS__)
#	define LOG_DEBUG(MSG,...) Raytracer::Logger::Write(4,MSG,##__VA_ARGS__)
#else
#	define LOG_LDEBUG(MSG,...) do{} while(0)
#	define LOG_DEBUG(MSG,...) do{} while(0)
#endif
#undef _DEBUG

#define LOG_INFO(MSG,...) Raytracer::Logger::Write(3,MSG,##__VA_ARGS__)
#define LOG_WARNING(MSG,...) Raytracer::Logger::Write(2,MSG,##__VA_ARGS__)
#define LOG_ERROR(MSG,...) Raytracer::Logger::Write(1,MSG,##__VA_ARGS__)
#define LOG_FATAL(MSG,...) Raytracer::Logger::Write(0,MSG,##__VA_ARGS__)

namespace Raytracer
{
	class Logger
	{
	public:
		static void Init(uint32_t logLevel);
		static void Write(uint32_t logLevel, const char* message, ...);

	};
}