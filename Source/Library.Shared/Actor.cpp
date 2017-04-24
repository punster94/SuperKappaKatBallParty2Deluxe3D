#include "pch.h"
#include "Actor.h"
#include "Renderer.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Actor)

	Actor::Actor()
	{
		InitializeSignatures();
	}

	Actor::~Actor()
	{

	}

	void Actor::SetPosition(glm::vec3 position)
	{
		mPosition = position;
	}

	void Actor::SetRotation(glm::vec3 rotation)
	{
		mRotation = rotation;
	}

	void Actor::SetScale(glm::vec3 scale)
	{
		mScale = scale;
	}

	glm::vec3 Actor::GetPosition() const
	{
		return mPosition;
	}

	glm::vec3 Actor::GetRotation() const
	{
		return mRotation;
	}

	glm::vec3 Actor::GetScale() const
	{
		return mScale;
	}

	void Actor::Update(WorldState& worldState)
	{
		Entity::Update(worldState);

		for (auto& renderable : mRenderables)
		{
			Mesh* mesh = renderable->As<Mesh>();

			if (mesh != nullptr)
			{
				mesh->Render(Renderer::Get());
			}
		}
	}

	void Actor::InitializeSignatures()
	{
		Entity::InitializeSignatures();
	}
}