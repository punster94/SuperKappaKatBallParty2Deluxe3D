#pragma once

#include <Windows.h>
#include "GLFW\glfw3.h"
#include "Renderer.h"

namespace FieaGameEngine
{
	class RendererOpenGL final : public Renderer
	{
	public:
		/**
		*	Initializes the renderer with the configuration settings.
		*/
		RendererOpenGL(RenderConfiguration& config);

		/**
		*	Initializes GLFW.
		*/
		virtual void Init() override;

		/**
		*	Initializes GLFW for rendering the frame.
		*/
		virtual void InitRenderFrame() override;
		
		/**
		*	Ends the frame by drawing to the screen.
		*/
		virtual void EndRenderFrame() override;

		/**
		*	Returns true if GLFW can continue to render the frame.
		*/
		virtual bool IsValid() override;

		/**
		*	Closes GLFW.
		*/
		virtual void Shutdown() override;

		virtual Mesh& CreateMesh(const std::string& meshPath) override;

	private:
		GLFWwindow* mWindow;
	};
}