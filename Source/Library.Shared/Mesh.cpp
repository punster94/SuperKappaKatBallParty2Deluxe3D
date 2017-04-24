#include "pch.h"
#include "Mesh.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Mesh)

	Mesh::Mesh() :
		mOwner(nullptr),
		mVisible(true)
	{

	}

	Mesh::Mesh(Entity& owner)
		: mOwner(&owner)
	{

	}

	void Mesh::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	void Mesh::SetOwner(Entity& owner)
	{
		mOwner = &owner;
	}

	void Mesh::SetRelativePosition(glm::vec3 position)
	{
		mRelativePosition = position;
	}

	void Mesh::SetRelativeRotation(glm::vec3 rotation)
	{
		mRelativeRotation = rotation;
	}

	void Mesh::SetRelativeScale(glm::vec3 scale)
	{
		mRelativeScale = scale;
	}
}