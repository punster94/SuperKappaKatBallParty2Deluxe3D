#include "pch.h"
#include "Renderable.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Renderable);

	Renderable::Renderable() :
		mVisible(true)
	{

	}

	bool Renderable::IsVisible()
	{
		return mVisible;
	}

	void Renderable::SetVisible(bool visible)
	{
		mVisible = visible;
	}
}