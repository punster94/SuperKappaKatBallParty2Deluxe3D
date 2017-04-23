#pragma once

#include "Renderer.h"
#include "Entity.h"

namespace FieaGameEngine
{
	class Renderable abstract
	{
	public:
		virtual void Render(Renderer& renderer) = 0;
	};
}