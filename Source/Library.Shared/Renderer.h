#pragma once

#include "Vector.h"
#include "Entity.h"

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
		*	Initializes the graphics systems.
		*/
		virtual void Init() = 0;

		/**
		*	Initializes the graphics systems for rendering a frame.
		*/
		virtual void InitRenderFrame() = 0;

		/**
		*	Renderers each renderable entity in a given World.
		*	@param world The World that contains Entities to render.
		*/
		void Render(const World& world);

		/**
		*	Renderers each renderable entity in a given Sector.
		*	@param sector The Sector that contains Entities to render.
		*/
		void Render(const Sector& sector);
	};
}