#pragma once

#include <cstdio>
#include <random>
#include <mutex>
#include <atomic>
#include <memory>
#include <cstring>
#include <algorithm>

#include "nmath.hpp"

//Software raytracer default config
#define RAY_MAX_DISTANCE	100000.0f
#define RAY_MIN_DISTANCE	0.001f
//--------------------------------

//Software raytracer helper typef
namespace Raytracer
{
	namespace Software
	{
		using namespace nova::math;

		//Declare classes
		class FramebufferObject;
		class TextureObject;
		class MaterialObject;
		class MeshObject;
		class SceneObject;

		//we will control memory explicitly from now
		
		//class Material;
		//typedef std::shared_ptr<Material> MaterialPtr;

		//class SceneLight;
		//typedef std::shared_ptr<SceneLight> SceneLightPtr;

		//class SceneObject;
		//typedef std::shared_ptr<SceneObject> SceneObjectPtr;
	}
}
//--------------------------------

//Software raytracer parts
#include "Utils.h"
#include "Random.h"
#include "Color.h"
#include "RayInfo.h"
#include "SurfaceHitInfo.h"
#include "FramebufferObject.h"
#include "TextureObject.h"
#include "MaterialObject.h"
#include "Scene.h"
#include "KernelConfig.h"
#include "TargetConfig.h"
#include "RegionConfig.h"
#include "SceneConfig.h"
#include "KernelInternals.h"

#include "Kernel.h"
//--------------------------------