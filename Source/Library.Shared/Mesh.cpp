#include "pch.h"
#include "Mesh.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Mesh)

	Mesh::Mesh()
	{

	}

	Mesh::Mesh(Actor& owner)
		: mOwner(&owner)
	{

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