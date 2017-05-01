#include "pch.h"
#include "ActionFocus.h"
#include "WorldState.h"

using namespace FieaGameEngine;
using namespace DirectX;

namespace KatBall
{
	RTTI_DEFINITIONS(ActionFocus)

	ActionFocus::ActionFocus(const std::string& name) :
		Action(name),
		mInitialized(false),
		mMinDistance(sDefaultMinDistance),
		mMaxDistance(sDefaultMaxDistance),
		mFocusScale(sDefaultFocusScale)
	{
		InitializeSignatures();
	}

	ActionFocus::ActionFocus(const ActionFocus& other) :
		Action(other),
		mMinDistance(sDefaultMinDistance),
		mMaxDistance(sDefaultMaxDistance),
		mFocusScale(sDefaultFocusScale)
	{
		FixExternalAttributes();
	}

	void ActionFocus::Update(WorldState& worldState)
	{
		if (!mInitialized)
		{
			Initialize(worldState);
		}

		glm::vec3 maxPosition;
		glm::vec3 minPosition;

		// Find the mid point of targets
		for (std::int32_t i = 0; i < mTargets.Size(); i++)
		{
			glm::vec3& position = mTargets[i]->GetWorldPosition();
			position.y = 0.0f;

			if (i == 0)
			{
				maxPosition = position;
				minPosition = position;
			}
			else
			{
				minPosition = glm::min(position, minPosition);
				maxPosition = glm::max(position, maxPosition);
			}
		}

		glm::vec3 midpoint = (minPosition + maxPosition) / 2.0f;
		float distance = glm::distance(minPosition, maxPosition) * mFocusScale;
		distance = glm::clamp(distance, mMinDistance, mMaxDistance);
		
		Entity* camera = GetParent()->As<Entity>();
		XMMATRIX rotationMatrix = XMMatrixIdentity();
		rotationMatrix *= XMMatrixRotationX(camera->GetWorldRotation().x);

		XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		forwardVector = XMVector4Transform(forwardVector, rotationMatrix);

		XMFLOAT4 forward4;
		XMStoreFloat4(&forward4, forwardVector);
		glm::vec3 forward(forward4.x, forward4.y, forward4.z);

		//forward = glm::normalize(forward);
		forward *= distance;
		camera->SetWorldPosition(midpoint - forward);
	}

	void ActionFocus::Initialize(WorldState& worldState)
	{
		// Find all entities matching target names;
		Datum targetStrings = (*this)[sTargetsKey];

		for (std::int32_t i = 0; i < targetStrings.Size(); i++)
		{
			std::string targetName = targetStrings.Get<std::string&>(i);
			Entity* entity = worldState.mSector->FindEntityByName(targetName);

			if (entity != nullptr)
			{
				mTargets.PushBack(entity);
			}
		}

		mInitialized = true;
	}

	Scope* ActionFocus::Copy() const
	{
		return new ActionFocus(*this);
	}

	void ActionFocus::InitializeSignatures()
	{
		AddExternalAttribute(sMinDistanceKey, &mMinDistance, 1);
		AddExternalAttribute(sMaxDistanceKey, &mMaxDistance, 1);
		AddExternalAttribute(sFocusScaleKey, &mFocusScale, 1);
		AddEmptyString(sTargetsKey);
	}

	void ActionFocus::FixExternalAttributes()
	{
		Append(sMinDistanceKey).SetStorage(&mMinDistance, 1);
		Append(sMaxDistanceKey).SetStorage(&mMaxDistance, 1);
		Append(sFocusScaleKey).SetStorage(&mFocusScale, 1);
	}

	const std::string ActionFocus::sTargetsKey = "target";
	const std::string ActionFocus::sMinDistanceKey = "min distance";
	const std::string ActionFocus::sMaxDistanceKey = "max distance";
	const std::string ActionFocus::sFocusScaleKey = "focus scale";

	const float ActionFocus::sDefaultMinDistance = 6.0f;
	const float ActionFocus::sDefaultMaxDistance = 20.0f;
	const float ActionFocus::sDefaultFocusScale = 0.5f;
}