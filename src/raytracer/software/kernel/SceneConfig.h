#pragma once

#include "SoftwareKernel.h"

namespace Raytracer
{
	namespace Software
	{
		struct SceneConfig
		{
			SceneConfig(
				SceneCamera& camera,
				SceneHierarchy* sceneHierarchy
			) :
				m_SceneCamera(camera),
				m_SceneHierarchy(sceneHierarchy)
			{};

			SceneConfig(SceneConfig const& s) :
				m_SceneCamera(s.m_SceneCamera),
				m_SceneHierarchy(s.m_SceneHierarchy)
			{};
			//--------------------------------
			SceneCamera& m_SceneCamera;
			SceneHierarchy* m_SceneHierarchy;
			//--------------------------------
		};
	}
}