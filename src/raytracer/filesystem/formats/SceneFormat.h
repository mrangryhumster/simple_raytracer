#pragma once

#include <cstdint>
#include <string>

#include "RaytracerLogger.h"
#include "RaytracerTexture.h"
#include "RaytracerSceneContainer.h"

namespace Raytracer
{
	namespace FileSystem
	{
		class SceneFormat
		{
		public:
			//Save RaytracerScene 
			static bool Export(std::string file_path, RaytracerSceneContainer* scene);
			//Load RaytracerScene
			static bool Import(std::string file_path, RaytracerSceneContainer* scene);
		};
	}
}