#include "RaytracerLogger.h"

#include <stdio.h>
#include <stdarg.h>

namespace Raytracer
{
	namespace __logger_internals
	{
		static uint32_t __logLevel;
	}

	void Logger::Init(uint32_t logLevel)
	{
		__logger_internals::__logLevel = logLevel;
	}

	void Logger::Write(uint32_t logLevel, const char* message, ...)
	{
		if (logLevel <= __logger_internals::__logLevel)
		{
			va_list args;
			va_start(args, message);
			vprintf(message, args);
			va_end(args);
		}
	}
}