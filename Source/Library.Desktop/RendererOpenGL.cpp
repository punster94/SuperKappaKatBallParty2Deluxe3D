#include "pch.h"
#include "RendererOpenGL.h"
#include "RenderConfigurationOpenGL.h"
#include "MeshOpenGL.h"

namespace FieaGameEngine
{
	RendererOpenGL::RendererOpenGL(RenderConfiguration& config)
		: Renderer(config)
	{
	}

	void RendererOpenGL::Init()
	{
		glfwInit();
		RenderConfigurationOpenGL* config = reinterpret_cast<RenderConfigurationOpenGL*>(mConfig);
		mWindow = glfwCreateWindow(config->windowWidth, config->windowHeight, config->windowName.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow);
	}

	void RendererOpenGL::InitRenderFrame()
	{
		glfwPollEvents();
	}

	void RendererOpenGL::EndRenderFrame()
	{
		glfwSwapBuffers(mWindow);
	}

	bool RendererOpenGL::IsValid()
	{
		return !glfwWindowShouldClose(mWindow);
	}

	void RendererOpenGL::Shutdown()
	{
	}

	Mesh& RendererOpenGL::CreateMesh(const std::string& meshPath)
	{
		meshPath;
		return *(new MeshOpenGL());
	}
}