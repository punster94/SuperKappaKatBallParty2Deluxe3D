#include "pch.h"
#include "RenderableDirectX.h"
#include "RendererDirectX.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(RenderableDirectX)

	void RenderableDirectX::Render(Renderer* renderer)
	{
		RendererDirectX* directX = reinterpret_cast<RendererDirectX*>(renderer);
		directX;
	}
}