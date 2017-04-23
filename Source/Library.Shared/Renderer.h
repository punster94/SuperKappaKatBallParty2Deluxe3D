#pragma once

#include "World.h"
#include "RenderConfiguration.h"
#include "Mesh.h"

namespace FieaGameEngine
{
	/**
	*	Abstract base class for a renderer.
	*	Handles global graphics-related tasks.
	*/
	class Renderer abstract
	{
	public:
		/**
		*	Initializes the renderer with the configuration settings.
		*/
		Renderer(RenderConfiguration& config);

		/**
		*	Initializes the graphics systems.
		*/
		virtual void Init() = 0;

		/**
		*	Initializes the graphics systems for rendering a frame.
		*/
		virtual void InitRenderFrame() = 0;

		/**
		*	Handles any additional functionality needed at the end of a frame.
		*/
		virtual void EndRenderFrame() = 0;

		/**
		*	True when the renderer can continue rendering, false otherwise.
		*/
		virtual bool IsValid() = 0;

		/**
		*	Destroys the graphics systems.
		*/
		virtual void Shutdown() = 0;

		/**
		*	Renderers each renderable entity in a given World.
		*	@param world The World that contains Entities to render.
		*/
		void Render(World& world);

		/**
		*	Renderers each renderable entity in a given Sector.
		*	@param sector The Sector that contains Entities to render.
		*/
		void Render(Sector& sector);

		/**
		*	Creates a mesh at the given path.
		*	@param The path where the mesh data exists.
		*	@return A reference to the created mesh.
		*/
		//virtual Mesh& CreateMesh(const std::string& meshPath) = 0;

	protected:
		RenderConfiguration* mConfig;
	};
}