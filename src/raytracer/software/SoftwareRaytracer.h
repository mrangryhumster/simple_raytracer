#pragma once

#include <cstdint>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>

#include "GL/gl3w.h"

#include "nmath.hpp"
#include "nthreading.hpp"

#include "RaytracerInterface.h"
#include "RaytracerTargetInterface.h"

#include "SoftwareKernel.h"

#define RENDER_TEXTURE_WIDTH 1024
#define RENDER_TEXTURE_HEIGHT 1024

namespace Raytracer
{
	namespace Software
	{
		using namespace nova::threading;
		using namespace nova::math;

		class SoftwareRaytracer : public RaytracerInterface
		{
		public:
			SoftwareRaytracer() = default;

			void Initialzie() override;
			void Destroy() override;
			void Update(bool updateTarget = true) override;

			void StartRaytracing(RaytracerTargetInterface* target, RaytracerSceneContainer* scene, RaytracerConfig const& config) override;
			void StopRaytracing() override;
			bool IsProcessing() override;
			const RaytracerStage* getState() override;
			const RaytracerPerformanceInfo& getPerformanceInfo() override;

		private:

			void UpdateDisplayTexture();

			bool ProcessSynchronizationStage();
			bool ProcessRenderingStage();
			bool ProcessFinalizationStage();

			
			// **** Called from another thread[s] ****
			void SyncScene();		//Final scene preparation
			void SyncRegions();		//Preparing rendering regions array
			void Finalization();	//Clean all mess we did
			// ***************************************

			//--------------------------------
			//Store all configs
			RaytracerConfig				m_RaytracerConfig;
			RaytracerSceneContainer*	m_RaytracerScene;
			//--------------------------------
			//RT kernel things
			KernelConfig				m_rtKernelConfig;
			std::vector<RegionConfig>   m_rtKernelRegions;
			//RT kernel scene
			color3f						 m_rtKernelAmbientColor;
			SceneCamera					 m_rtKernelCamera;
			std::vector<TextureObject*>	 m_rtKernelTextures;
			std::vector<MaterialObject*> m_rtKernelMaterials;
			std::vector<SceneObject*>	 m_rtKernelSceneObjects;
			SceneHierarchy*				 m_rtKernelSceneHierarchy;
			//--------------------------------
			//Render target output
			RaytracerTargetInterface*	m_RaytracingTarget;
			//Internal Render target (Actually reference real target data)
			FramebufferObject				m_RaytracingFramebuffer;
			//--------------------------------
			//Thing to keep track activity
			bool				  m_RenderingIsActive;
			bool				  m_RenderingIsStopping;
			RaytracerStage*		  m_RaytracerStage;
			//--------------------------------
			//Thread pool to processing tasks
			thread_pool_t<void* ,size_t>    m_ThreadPool;
			//Used to notify kernel if we need terminate current rendering tasks
			std::atomic_bool m_TerminationFlag;
			//--------------------------------
			//Timestamps
			std::chrono::high_resolution_clock::time_point	m_RenderingBeginTimestamp;
			std::chrono::high_resolution_clock::time_point	m_RenderingStageTimestamp;
			RaytracerPerformanceInfo m_RaytracerPerformanceInfo;
			//--------------------------------
		};
	}
}