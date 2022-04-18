#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct SceneConfig
		{
			SceneConfig(
				color3f& ambientColor,
				SceneCamera& camera,
				SceneHierarchy* sceneHierarchy
			) :
				m_AmbientColor(ambientColor),
				m_SceneCamera(camera),
				m_SceneHierarchy(sceneHierarchy)
			{};

			SceneConfig(SceneConfig const& s) :
				m_AmbientColor(s.m_AmbientColor),
				m_SceneCamera(s.m_SceneCamera),
				m_SceneHierarchy(s.m_SceneHierarchy)
			{};
			//--------------------------------
			color3f			m_AmbientColor;
			SceneCamera&	m_SceneCamera;
			SceneHierarchy* m_SceneHierarchy;
			//--------------------------------
		};
	}
}