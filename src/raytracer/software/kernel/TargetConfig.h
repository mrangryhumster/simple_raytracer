#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct TargetConfig
		{
			TargetConfig(FramebufferObject& frameBuffer) :
				m_Framebuffer(frameBuffer)
			{};

			TargetConfig(const TargetConfig& t) :
				m_Framebuffer(t.m_Framebuffer)
			{};

			//--------------------------------
			FramebufferObject&		m_Framebuffer;
			//--------------------------------
		};
	}
}