#pragma once

#include "Entity.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	class Renderer;

	class Renderable abstract : public RTTI 
	{
		RTTI_DECLARATIONS(Renderable, RTTI)

	public:
		virtual void Render(Renderer* renderer) = 0;
	};
}