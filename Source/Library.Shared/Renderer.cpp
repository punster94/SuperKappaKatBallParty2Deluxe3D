#include "pch.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Camera.h"

namespace FieaGameEngine
{
	Renderer* Renderer::sInstance = nullptr;

	const glm::vec4 Renderer::sDefaultLightDirection(1.0f, -1.0f, 1.0f, 0.0f);
	const glm::vec4 Renderer::sDefaultAmbientLight(0.2f, 0.2f, 0.2f, 1.0f);

	Renderer* Renderer::Get()
	{
		return sInstance;
	}

	void Renderer::Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	Renderer::Renderer(RenderConfiguration& config)
		: mConfig(&config),
		mLightDirection(sDefaultLightDirection),
		mAmbientLight(sDefaultAmbientLight)
	{
	}

	void Renderer::Render(World& world)
	{
		Datum& sectors = world.Sectors();
		for (std::uint32_t i = 0; i < sectors.Size(); ++i)
		{
			Render(static_cast<Sector&>(sectors.Get<Scope&>(i)));
		}
	}

	void Renderer::Render(Sector& sector)
	{
		Datum& entities = sector.Entities();
		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			static_cast<Entity&>(entities.Get<Scope&>(i)).Render(this);
		}
	}

	void Renderer::SetDepthTesting(bool enabled)
	{
		enabled;
	}

	void Renderer::SetCamera(Camera* camera)
	{
		camera;
	}

	void Renderer::AddViewRenderable(Renderable& renderable)
	{
		renderable;
	}

	void Renderer::RemoveViewRenderable(Renderable& renderable)
	{
		renderable;
	}
}