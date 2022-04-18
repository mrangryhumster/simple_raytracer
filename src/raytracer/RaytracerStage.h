#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace Raytracer
{
	//----------------------------------------------------------------
	//Base class for stages
	class RaytracerStage
	{
	public:
		virtual ~RaytracerStage() = default;
		//----------------
		//internal structures declaration
		enum class Stage
		{
			Ready,
			Synchronization,
			Rendering,
			Finalization
		};
		//----------------
		//hints to which struct RenderingConfig should be casted to get info
		Stage m_Stage;
	protected:
		RaytracerStage(RaytracerStage::Stage s) : m_Stage(s) {};
	};
	//----------------------------------------------------------------
	//This stage should be always set when raytracer is not doing anything
	class RaytracerReadyStage : public RaytracerStage
	{
	public:
		RaytracerReadyStage() : RaytracerStage(Stage::Ready) {};
		virtual ~RaytracerReadyStage() = default;
	};
	//----------------------------------------------------------------
	//This stage showing that raytracer is in sync stage which means:
	//	preparing internals structures, converting app scene container to internal format
	//  doing some other things
	class RaytracerSynchronizationStage : public RaytracerStage
	{
	public:
		RaytracerSynchronizationStage() : RaytracerStage(Stage::Synchronization) {};
		virtual ~RaytracerSynchronizationStage() = default;
	};
	//----------------------------------------------------------------
	//This stage showing that raytracer is rendering target regions
	class RaytracerRenderingRegionsStage : public RaytracerStage
	{
	public:
		RaytracerRenderingRegionsStage() : RaytracerStage(Stage::Rendering) {};
		virtual ~RaytracerRenderingRegionsStage() = default;
		//----------------
		struct RenderingRegion
		{
			RenderingRegion() = default;
			RenderingRegion(uint32_t _x, uint32_t _y, uint32_t _w, uint32_t _h) :
				x(_x), y(_y), w(_w), h(_h)
			{};
			uint32_t x, y;
			uint32_t w, h;
		};
		//----------------
		std::vector<RenderingRegion> m_Regions;
		uint32_t m_RegionsQueued;
		uint32_t m_RegionsCount;
		//----------------
	};
	//----------------------------------------------------------------
	//This stage showing that raytracer is... finishing its job...
	class RaytracerFinalizationStage : public RaytracerStage
	{
	public:
		RaytracerFinalizationStage() : RaytracerStage(Stage::Finalization) {};
		virtual ~RaytracerFinalizationStage() = default;
	};
	//----------------------------------------------------------------
}