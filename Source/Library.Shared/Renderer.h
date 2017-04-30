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

		virtual ~Renderer() = default;

		/**
		*	Gets the singleton
		*/
		static Renderer* Get();

		/**
		*	Destroys the singleton
		*/
		static void Destroy();

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

		virtual void SetDepthTesting(bool enabled);

		/**
		*	Renderers each renderable entity in a given World.
		*	@param world The World that contains Entities to render.
		*/
		virtual void Render(World& world);

		virtual void AddViewRenderable(Renderable& renderable);

		virtual void RemoveViewRenderable(Renderable& renderable);
		/**
		*	Renderers each renderable entity in a given Sector.
		*	@param sector The Sector that contains Entities to render.
		*/
		void Render(Sector& sector);

		virtual void SetCamera(class Camera* camera);

		void SetLightDirection(const glm::vec4& lightDirection);

	protected:

		static const glm::vec4 sDefaultLightDirection;
		static const glm::vec4 sDefaultAmbientLight;

		/**
		*	Initializes the renderer with the configuration settings.
		*/
		Renderer(RenderConfiguration& config);

		static Renderer* sInstance;

		RenderConfiguration* mConfig;

		glm::vec4 mLightDirection;

		glm::vec4 mAmbientLight;
	};
}