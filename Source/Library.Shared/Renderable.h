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
		
		Renderable();

		virtual ~Renderable() = default;

		virtual void Render(Renderer* renderer) = 0;

		bool IsVisible();

		void SetVisible(bool visible);

	protected:

		bool mVisible;
	};
}