#include "pch.h"
#include "Mesh.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Mesh)

	Mesh::Mesh() :
		mOwner(nullptr),
		mVisible(false)
	{

	}

	Mesh::Mesh(Actor& owner)
		: mOwner(&owner)
	{

	}

	void Mesh::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	void Mesh::SetOwner(Actor& owner)
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