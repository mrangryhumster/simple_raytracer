#pragma once

#include <cstdint>
#include <cstdio>
#include <limits>
#include <cmath>
#include <thread>

//third party libs
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// ^ thank you ^ \(^_^)/

#include "GUIApplicationDefaults.h"

//Logs
#include "RaytracerLogger.h"

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

class GUIApplication
{
public:
	int Run(int argc, char** argv);
protected:
	int Initialize();
	void Destroy();

	int MainLoop();

	void DrawGui();
	void DrawRenderTab();

private:
	static void resize_window_callback(GLFWwindow* glfw_window, int x, int y);


	Raytracer::RaytracerInterface*			m_Raytracer;
	Raytracer::RaytracerConfig				m_RaytracerConfig;
	Raytracer::RaytracerTargetInterface*	m_RaytracerTarget;
	Raytracer::RaytracerSceneContainer*		m_RaytracerScene;

	uint32_t m_RaytracerTargetWidth, m_RaytracerTargetHeight;

	GLFWwindow* m_glfwWindow;
	uint32_t	m_lastWindowWidth;
	uint32_t	m_lastWindowHeight;

	//RenderTabThings
	ImVec2	m_RenderTab_Offset;
	float	m_RenderTab_Scale;
};