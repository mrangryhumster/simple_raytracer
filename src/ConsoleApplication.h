#pragma once

#include <cstdint>
#include <cstdio>
#include <limits>
#include <cmath>

#include "ConsoleApplicationDefaults.h"

//Interfaces
#include "RaytracerInterface.h"
#include "RaytracerTargetInterface.h"

//Scene container
#include "RaytracerSceneContainer.h"

//File system 
#include "FileSystem.h"

//Implementations
#include "OpenGLRaytracerTarget.h"
#include "NullRaytracerTarget.h"
#include "SoftwareRaytracer.h"

//----------------------------------------

class ConsoleApplication
{
public:
	int Run(int argc, char** argv);
	int NormalRun(int argc, char** argv);
	int ProfilerRun(int argc, char** argv);
};