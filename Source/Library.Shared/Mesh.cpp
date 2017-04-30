#include "pch.h"
#include "Mesh.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Mesh)

	Mesh::Mesh() :
		mOwner(nullptr),
		mRenderPass(RENDER_PASS_1)
	{

	}

	Mesh::Mesh(Entity& owner)
		: mOwner(&owner)
	{

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

	void Mesh::AddRelativePosition(glm::vec3 deltaPosition)
	{
		mRelativePosition += deltaPosition;
	}

	void Mesh::AddRelativeRotation(glm::vec3 deltaRotation)
	{
		mRelativeRotation += deltaRotation;
	}

	void Mesh::AddRelativeScale(glm::vec3 deltaScale)
	{
		mRelativeScale += deltaScale;
	}

	std::int32_t Mesh::GetRenderPass()
	{
		return mRenderPass;
	}

	void Mesh::SetRenderPass(int32_t renderPass)
	{
		if (renderPass > RENDER_PASS_2)
			renderPass = RENDER_PASS_2;
		if (renderPass < RENDER_PASS_1)
			renderPass = RENDER_PASS_1;

		mRenderPass = renderPass;
	}
}