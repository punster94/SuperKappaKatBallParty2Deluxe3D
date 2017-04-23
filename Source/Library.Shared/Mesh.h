#pragma once

#include "Renderable.h"

namespace FieaGameEngine
{
	class Mesh abstract : public Renderable
	{
	public:
		virtual void Render(Renderer& renderer) override = 0;
	};
}