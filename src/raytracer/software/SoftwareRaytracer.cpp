#include "SoftwareRaytracer.h"

namespace Raytracer
{
	namespace Software
	{
		void SoftwareRaytracer::Initialzie()
		{
			m_ThreadPool.resize(thread_pool::hardware_concurrency);
			m_RenderingIsActive = false;
			m_RenderingIsStopping = false;
			m_RaytracerStage = new RaytracerReadyStage();
			m_RaytracingTarget = nullptr;

			m_rtKernelSceneHierarchy = nullptr;
		}

		void SoftwareRaytracer::Destroy()
		{
			delete m_RaytracerStage;
		}

		void SoftwareRaytracer::Update(bool updateTarget)
		{
			if (m_RenderingIsActive)
			{
				//Process queued tasks and go to our ugly state machine
				m_ThreadPool.process();

				//If Process***Stage returns false (which means "stage unable or done"
				//we will fall into next case, ugly but fun
								
				switch (m_RaytracerStage->m_Stage)
				{
				case RaytracerStage::Stage::Ready: //First stage is simple, record time and fall further				 
					m_RenderingBeginTimestamp = std::chrono::high_resolution_clock::now();

				// State processing begin
				case RaytracerStage::Stage::Synchronization:	if (ProcessSynchronizationStage())	break;
				case RaytracerStage::Stage::Rendering:			if (ProcessRenderingStage())		break;
				case RaytracerStage::Stage::Finalization:		if (ProcessFinalizationStage())		break;
				// State processing end

				default: //We done or failed miserably, so stopping further actions and calc time
					m_RenderingIsActive = false;
					m_RaytracerPerformanceInfo.m_StageTime.m_TotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::high_resolution_clock::now() - m_RenderingBeginTimestamp
						).count() * 0.001f;
					LOG_LDEBUG("FRAME TIME : %f\n", m_RaytracerPerformanceInfo.m_StageTime.m_TotalTime);
					break;
				}
			}
			if(updateTarget)
				UpdateDisplayTexture();
		}

		void SoftwareRaytracer::StartRaytracing(RaytracerTargetInterface* target, RaytracerSceneContainer* scene, RaytracerConfig const& config)
		{
			if (m_RenderingIsActive) return;
			//Store configs 
			m_RaytracerConfig = config;
			m_RaytracingTarget = target;
			m_RaytracerScene = scene;

			//Convert setting to Kernel settings
			m_rtKernelConfig.m_RayMaxBounces = m_RaytracerConfig.m_RayMaxBounces;
			m_rtKernelConfig.m_TerminateFlag = &m_TerminationFlag;

			//Prepare kernel output buffer
			m_RaytracingFramebuffer.Link(
				m_RaytracingTarget->getWidth(),
				m_RaytracingTarget->getHeight(),
				m_RaytracingTarget->getData()
			);

			//Prepare thread pool
			m_ThreadPool.resize((config.m_Threads == 0) ? thread_pool::hardware_concurrency : config.m_Threads);

			//Clear performance info
			m_RaytracerPerformanceInfo.m_StageTime.m_SynchronizationTime = 0.f;
			m_RaytracerPerformanceInfo.m_StageTime.m_RenderingTime = 0.f;
			m_RaytracerPerformanceInfo.m_StageTime.m_FinalizationTime = 0.f;
			m_RaytracerPerformanceInfo.m_StageTime.m_TotalTime = 0.f;

			//Make sure - our first stage is ReadyStage (delete if there another stage)
			if (m_RaytracerStage != nullptr) delete m_RaytracerStage;
			m_RaytracerStage = new RaytracerReadyStage();

			//Here we go!
			m_TerminationFlag = false;
			m_RenderingIsStopping = false;
			m_RenderingIsActive = true; 
		}

		void SoftwareRaytracer::StopRaytracing()
		{
			if (!m_RenderingIsActive) return;
			m_TerminationFlag.store(true);
			m_RenderingIsStopping = true;
		}

		bool SoftwareRaytracer::IsProcessing()
		{
			return m_RenderingIsActive;
		}

		const RaytracerStage* SoftwareRaytracer::getState()
		{
			return m_RaytracerStage;
		}

		const RaytracerPerformanceInfo& SoftwareRaytracer::getPerformanceInfo()
		{
			return m_RaytracerPerformanceInfo;
		}

		void SoftwareRaytracer::UpdateDisplayTexture()
		{
			if (m_RaytracingTarget != nullptr)
			{
				m_RaytracingTarget->Sync(true);
			}
		}

		//********************************************************************************************************************************
		//********************************************************************************************************************************
		//********************************************************************************************************************************

		bool SoftwareRaytracer::ProcessSynchronizationStage()
		{
			if (m_RaytracerStage->m_Stage != RaytracerStage::Stage::Synchronization)
			{
				LOG_LDEBUG("%s\n", __FUNCTION__);
				delete m_RaytracerStage; //Change old state by deleting it and allocating new
				m_RaytracerStage = new RaytracerSynchronizationStage();
				//Update timestamp
				m_RenderingStageTimestamp = std::chrono::high_resolution_clock::now();
				//----------------------------------------------------------------

				//Pushing task to prepare scene
				m_ThreadPool.push_task(nullptr, &SoftwareRaytracer::SyncScene, this);

				//Pushing task to prepare regions
				m_ThreadPool.push_task(nullptr, &SoftwareRaytracer::SyncRegions, this);
				//----------------------------------------------------------------
				return true;
			}
			else if (m_ThreadPool.tasks_count() > 0)
			{
				//still processing
				return true;
			}

			//We done here, record time and get out
			m_RaytracerPerformanceInfo.m_StageTime.m_SynchronizationTime = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - m_RenderingStageTimestamp
				).count() * 0.001f;
			return false;
		}

		bool Raytracer::Software::SoftwareRaytracer::ProcessRenderingStage()
		{
			if (m_RaytracerStage->m_Stage != RaytracerStage::Stage::Rendering)
			{
				LOG_LDEBUG("%s\n", __FUNCTION__);
				delete m_RaytracerStage; //Change old state by deleting it and allocating new
				m_RaytracerStage = new RaytracerRenderingRegionsStage();

				//Initial data
				reinterpret_cast<RaytracerRenderingRegionsStage*>(m_RaytracerStage)->m_Regions.resize(m_ThreadPool.threads_count());
				reinterpret_cast<RaytracerRenderingRegionsStage*>(m_RaytracerStage)->m_RegionsCount = static_cast<uint32_t>(m_rtKernelRegions.size());
				reinterpret_cast<RaytracerRenderingRegionsStage*>(m_RaytracerStage)->m_RegionsQueued = 0;
				//Update timestamp
				m_RenderingStageTimestamp = std::chrono::high_resolution_clock::now();
				//----------------------------------------------------------------
				//Pushing tasks to draw regions
				for (int i = 0; i < m_rtKernelRegions.size(); i++)
				{
					m_ThreadPool.push_task(
						&m_rtKernelRegions[i],
						Raytracer::Software::Kernel::Raytrace,
						m_rtKernelConfig,
						TargetConfig(m_RaytracingFramebuffer),
						m_rtKernelRegions[i],
						SceneConfig(m_rtKernelCamera, m_rtKernelSceneHierarchy)
					);
				}
				//----------------------------------------------------------------
				return true;
			}
			else if (m_ThreadPool.tasks_count() > 0) //Still processing tasks
			{
				if (!m_RenderingIsStopping)
				{
					//Updating stage info from active tasks
					size_t threadCount = m_ThreadPool.threads_count();
					for (size_t idx = 0; idx < threadCount; idx++)
					{
						auto shared = reinterpret_cast<RegionConfig*>(m_ThreadPool.shared(idx));

						if (shared != nullptr)
							reinterpret_cast<RaytracerRenderingRegionsStage*>(m_RaytracerStage)->m_Regions[idx] =
							RaytracerRenderingRegionsStage::RenderingRegion(shared->m_StartX, shared->m_StartY, shared->m_Width, shared->m_Height);
					}
					reinterpret_cast<RaytracerRenderingRegionsStage*>(m_RaytracerStage)->m_RegionsQueued = static_cast<uint32_t>(m_ThreadPool.tasks_count());
				}
				else
				{
					m_ThreadPool.clear_tasks();
				}
				return true;
			}
				
			//We done here, record time and get out
			m_RaytracerPerformanceInfo.m_StageTime.m_RenderingTime = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - m_RenderingStageTimestamp
				).count() * 0.001f;
			return false;
		}

		bool SoftwareRaytracer::ProcessFinalizationStage()
		{
			if (m_RaytracerStage->m_Stage != RaytracerStage::Stage::Finalization)
			{
				LOG_LDEBUG("%s\n", __FUNCTION__);
				delete m_RaytracerStage; //Change old state by deleting it and allocating new
				m_RaytracerStage = new RaytracerFinalizationStage();
				//Update timestamp
				m_RenderingStageTimestamp = std::chrono::high_resolution_clock::now();

				//----------------------------------------------------------------
				
				//Final task to cleanup all mess we did in memory
				m_ThreadPool.push_task(nullptr, &SoftwareRaytracer::Finalization, this);

				//----------------------------------------------------------------
				return true;
			}
			else if (m_ThreadPool.tasks_count() > 0) //Still processing tasks
			{
				return true;
			}

			//We done here, record time and get out
			m_RaytracerPerformanceInfo.m_StageTime.m_FinalizationTime = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - m_RenderingStageTimestamp
				).count() * 0.001f;
			return false;
		}

		//********************************************************************************************************************************
		//********************************************************************************************************************************
		//********************************************************************************************************************************
		void SoftwareRaytracer::SyncScene()
		{			
			//Even if possible split geometry bvh creation between threads
			//we using a single-threaded variant bacause its simplier

			//Requesting scene container arrays
			auto textureArray = m_RaytracerScene->getTextureArray();
			auto materialArray = m_RaytracerScene->getMaterialArray();
			auto meshArray = m_RaytracerScene->getMeshArray();
			auto objectArray = m_RaytracerScene->getObjectArray();

			//Allocating some maps to resolve all references inside container on plain arrays
			std::map<uint64_t, size_t>  texturesIdToIndex;
			std::map<uint64_t, size_t>  materialsIdToIndex;
			std::map<uint64_t, size_t>  meshIdToIndex;
			std::map<uint64_t, size_t>  objectIdToIndex;

			//Reserving space in kernel arrays
			m_rtKernelTextures.reserve(textureArray.size());
			m_rtKernelMaterials.reserve(materialArray.size());
			m_rtKernelSceneObjects.reserve(objectArray.size());

			// --- Materials stage ---
			 
			//Processing container textures
			for (size_t idx = 0; idx < textureArray.size(); idx++)
			{
				const auto texture = textureArray[idx];
				texturesIdToIndex[texture->m_Id] = m_rtKernelTextures.size();

				auto rtTexture = new TextureObject();
				rtTexture->Initialize(
					texture->m_Width,
					texture->m_Height,
					texture->m_Components,
					texture->m_Data,
					texture->m_DataSize,
					true
				);
				m_rtKernelTextures.push_back(rtTexture);
			}
			
			//Processing container materials

			//Simple macro to remove most of the copypaste code 
			//(it works because kernel and upper level have similar structure for materials)
			
//MACRO BEGIN
#			define MACRO_CONVERT_SIMILAR_FIELD(field,src,dst)													\
			dst->m_Is##field##Texture = src->m_Is##field##Texture;												\
			if(!dst->m_Is##field##Texture) dst->m_##field##.Value = src->m_##field##.Value;						\
			else dst->m_##field##.Texture = m_rtKernelTextures[texturesIdToIndex[src->m_##field##.TextureId]];	\
//MACRO END

			for (size_t idx = 0; idx < materialArray.size(); idx++)
			{
				const auto material = materialArray[idx];
				materialsIdToIndex[material->m_Id] = m_rtKernelMaterials.size();

				auto rtMaterial = new MaterialObject();

				MACRO_CONVERT_SIMILAR_FIELD(Albedo,				material, rtMaterial);
				MACRO_CONVERT_SIMILAR_FIELD(Metallic,			material, rtMaterial);
				MACRO_CONVERT_SIMILAR_FIELD(Roughness,			material, rtMaterial);
				MACRO_CONVERT_SIMILAR_FIELD(IOR,				material, rtMaterial);
				MACRO_CONVERT_SIMILAR_FIELD(Transmission,		material, rtMaterial);
				MACRO_CONVERT_SIMILAR_FIELD(EmissionStrength,	material, rtMaterial);
				MACRO_CONVERT_SIMILAR_FIELD(EmissionColor,		material, rtMaterial);
	
				m_rtKernelMaterials.push_back(rtMaterial);
			}

//MACRO BEGIN
#			undef MACRO_CONVERT_SIMILAR_FIELD
//MACRO END

			// --- BVH and co. stage ---

			//Processing container meshes
			for (size_t idx = 0; idx < meshArray.size(); idx++)
			{
				meshIdToIndex[meshArray[idx]->m_Id] = idx;
			}

			//Processing container objects 
			for (size_t idx = 0; idx < objectArray.size(); idx++)
			{
				const auto object = objectArray[idx];

				const auto mesh = m_RaytracerScene->getMesh(object->m_MeshId).get();
				const auto rtMaterial = m_rtKernelMaterials[materialsIdToIndex[object->m_MaterialId]];

				switch (mesh->m_Type)
				{
				case RaytracerMeshType::Empty:
					break;
				case RaytracerMeshType::Sphere:
					m_rtKernelSceneObjects.push_back(
						new SceneObjectSphere(
							object->m_Position,
							static_cast<RaytracerMeshSphere*>(mesh)->m_Radius,
							rtMaterial
						));
					break;
				}
			}
			//-------------------------------------------------------------------
			const auto mainCamera = m_RaytracerScene->getMainCamera();
			m_rtKernelCamera.setView(
				mainCamera->m_Position,
				mainCamera->m_Target,
				mainCamera->m_Up,
				mainCamera->m_FOV
			);
			m_rtKernelCamera.setAspect(static_cast<float>(m_RaytracingTarget->getWidth()) / static_cast<float>(m_RaytracingTarget->getHeight()));
			m_rtKernelCamera.Build();
			//-------------------------------------------------------------------
			switch (m_RaytracerConfig.m_Internal_SoftwareBVHType)
			{
			case 0: m_rtKernelSceneHierarchy = new SceneHierarchyBVH(); break;
			case 1: m_rtKernelSceneHierarchy = new SceneHierarchyBVH_RTIOW(); break;
			default:
				m_rtKernelSceneHierarchy = new SceneHierarchyBVH();
				break;
			}

			m_rtKernelSceneHierarchy->Build(m_rtKernelSceneObjects.data(), m_rtKernelSceneObjects.size());
			//-------------------------------------------------------------------
		}

		void SoftwareRaytracer::SyncRegions()
		{
			//------------------------------------------------------------------------------
			uint32_t targetWidth = m_RaytracingTarget->getWidth();
			uint32_t targetHeight = m_RaytracingTarget->getHeight();
			//------------------------------------------------------------------------------
			uint32_t xTiles = targetWidth / m_RaytracerConfig.m_TileWidth;
			uint32_t yTiles = targetHeight / m_RaytracerConfig.m_TileHeight;
			uint32_t xTileReminder = targetWidth % m_RaytracerConfig.m_TileWidth;
			uint32_t yTileReminder = targetHeight % m_RaytracerConfig.m_TileHeight;
			//------------------------------------------------------------------------------
			size_t regionsCount = xTiles * yTiles;
			if (xTileReminder > 0) regionsCount += xTiles;
			if (yTileReminder > 0) regionsCount += yTiles;
			if (xTileReminder > 0 && yTileReminder > 0) regionsCount += 1;
			//------------------------------------------------------------------------------
			m_rtKernelRegions.clear();
			m_rtKernelRegions.reserve(regionsCount);
			//------------------------------------------------------------------------------
			//Main tasks
			for (uint32_t y = 0; y < yTiles; y++)
				for (uint32_t x = 0; x < xTiles; x++)
					m_rtKernelRegions.push_back(RegionConfig(
						(m_RaytracerConfig.m_TileWidth * x), (m_RaytracerConfig.m_TileHeight * y),
						m_RaytracerConfig.m_TileWidth, m_RaytracerConfig.m_TileHeight,
						m_RaytracerConfig.m_SamplesPerPixel
					));
			//------------------------------------------------------------------------------
			//LastRow
			if (yTileReminder > 0)
				for (uint32_t x = 0; x < xTiles; x++)
					m_rtKernelRegions.push_back(RegionConfig(
						(m_RaytracerConfig.m_TileWidth * x), (m_RaytracerConfig.m_TileHeight * yTiles),
						m_RaytracerConfig.m_TileWidth, yTileReminder,
						m_RaytracerConfig.m_SamplesPerPixel
					));
			//LastColumn
			if (xTileReminder > 0)
				for (uint32_t y = 0; y < yTiles; y++)
					m_rtKernelRegions.push_back(RegionConfig(
						(m_RaytracerConfig.m_TileWidth * xTiles), (m_RaytracerConfig.m_TileHeight * y),
						xTileReminder, m_RaytracerConfig.m_TileHeight,
						m_RaytracerConfig.m_SamplesPerPixel
					));
			//CornerTile
			if (xTileReminder > 0 && yTileReminder > 0)
				m_rtKernelRegions.push_back(RegionConfig(
					(m_RaytracerConfig.m_TileWidth * xTiles), (m_RaytracerConfig.m_TileHeight * yTiles),
					xTileReminder, yTileReminder,
					m_RaytracerConfig.m_SamplesPerPixel
				));
			//------------------------------------------------------------------------------
			//Sort from center
			const vec3::type center_x = static_cast<vec3::type>(targetWidth / 2.0f);
			const vec3::type center_y = static_cast<vec3::type>(targetHeight / 2.0f);
			std::sort(std::begin(m_rtKernelRegions), std::end(m_rtKernelRegions), [&](RegionConfig const& a, RegionConfig const& b) {

				return distance_squared(vec3(center_x, center_y, 0), vec3(static_cast<vec3::type>(a.m_StartX), static_cast<vec3::type>(a.m_StartY), 0.0f)) <
					distance_squared(vec3(center_x, center_y, 0), vec3(static_cast<vec3::type>(b.m_StartX), static_cast<vec3::type>(b.m_StartY), 0.0f))
					;
				}
			);
			//------------------------------------------------------------------------------
		}

		void SoftwareRaytracer::Finalization()
		{
			//Clear regions
			m_rtKernelRegions.clear(); //actually not needed, just to be safe
			m_rtKernelRegions.resize(0);

			//Delete images
			for (size_t i = 0; i < m_rtKernelTextures.size(); i++)
				if(m_rtKernelTextures[i] != nullptr)
					delete m_rtKernelTextures[i];
			m_rtKernelTextures.resize(0);

			//Delete materials
			for (size_t i = 0; i < m_rtKernelMaterials.size(); i++)
				if (m_rtKernelMaterials[i] != nullptr)
					delete m_rtKernelMaterials[i];
			m_rtKernelMaterials.resize(0);

			//Delete scene objects
			for (size_t i = 0; i < m_rtKernelSceneObjects.size(); i++)
				if (m_rtKernelSceneObjects[i] != nullptr)
					delete m_rtKernelSceneObjects[i];
			m_rtKernelSceneObjects.resize(0);

			m_rtKernelSceneHierarchy->Clear();
			delete m_rtKernelSceneHierarchy;
		}
	}
}