#include "pch.h"
#include "Renderer.h"
#include "Renderable.h"

namespace FieaGameEngine
{
	void Renderer::Render(const World& world)
	{
		InitRenderFrame();

		const Datum& sectors = world.Sectors();
		for (std::uint32_t i = 0; i < sectors.Size(); ++i)
		{
			Render(static_cast<Sector&>(sectors.Get<Scope&>(i)));
		}
	}

	void Renderer::Render(const Sector& sector)
	{
		const Datum& entities = sector.Entities();
		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			Renderable* renderable = static_cast<Entity&>(entities.Get<Scope&>(i)).As<Renderable>();
			if (renderable != nullptr)
			{
				renderable->Render(this);
			}
		}
	}
}