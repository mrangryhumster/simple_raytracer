#pragma once

#include "RaytracerConfig.h"
#include "RaytracerStage.h"
#include "RaytracerSceneContainer.h"
#include "RaytracerPerformanceInfo.h"
#include "RaytracerTargetInterface.h"

namespace Raytracer
{
	class RaytracerInterface
	{
	public:
		virtual ~RaytracerInterface() = default;

		virtual void Initialzie() = 0;
		virtual void Destroy() = 0;
		virtual void Update(bool updateTarget = true) = 0;

		//WARNING! do not change state of target or scene durning rendering or you will face undefined behavior up to app crash
		virtual void StartRaytracing(RaytracerTargetInterface* target, RaytracerSceneContainer* scene, RaytracerConfig const& config) = 0;
		virtual void StopRaytracing() = 0;
		virtual bool IsProcessing() = 0;
		virtual const RaytracerStage* getState() = 0;
		virtual const RaytracerPerformanceInfo& getPerformanceInfo() = 0;
	};
}