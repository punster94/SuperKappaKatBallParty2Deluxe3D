#pragma once

#include "Renderer.h"
#include "Entity.h"

namespace FieaGameEngine
{
	class Renderable abstract : public Entity
	{
		RTTI_DECLARATIONS(Renderable, Entity)

	public:
		virtual void Render(Renderer* renderer) = 0;
	};
}