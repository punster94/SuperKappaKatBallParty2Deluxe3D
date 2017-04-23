#pragma once

#include "Entity.h"

namespace FieaGameEngine
{
	class Renderer;

	class Renderable abstract
	{
	public:
		virtual void Render(Renderer* renderer) = 0;
	};
}