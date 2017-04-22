#pragma once

#include "Renderable.h"

namespace FieaGameEngine
{
	class RenderableDirectX : public Renderable
	{
		RTTI_DECLARATIONS(RenderableDirectX, Renderable)

	public:
		virtual void Render(Renderer* renderer);
	};
}