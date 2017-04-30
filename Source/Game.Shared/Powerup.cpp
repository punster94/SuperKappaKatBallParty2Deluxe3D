#include "pch.h"
#include "Powerup.h"

namespace KatBall
{
	Powerup::Powerup(PowerupType type, std::float_t stat, glm::vec4 location) :
		Entity(mName), mType(type), mSpawnLocation(location), mScaleIncrease(0), mLengthIncrease(0), mRotationSpeed(0)
	{
		switch (type)
		{
		case BigBoi:
			mScaleIncrease = stat;
			break;
		case LongBoi:
			mLengthIncrease = stat;
			break;
		case VortexBoi:
			mRotationSpeed = stat;
			break;
		}

		InitializeSignatures();
	}

	void Powerup::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute(mScaleIncreaseKey, &mScaleIncrease, 1);
		AddExternalAttribute(mLengthIncreaseKey, &mLengthIncrease, 1);
		AddExternalAttribute(mRotationSpeedKey, &mRotationSpeed, 1);
		AddExternalAttribute(mSpawnLocationKey, &mSpawnLocation, 1);
	}

	void Powerup::Initialize(FieaGameEngine::WorldState& worldState)
	{
		Entity::Initialize(worldState);

		mMeshEntity = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();
		mRigidBody = FindChildEntityByName(sRigidBodyKey)->As<RigidBody>();
	}

	void Powerup::OnCollect(Player& katBoi)
	{
		switch (mType)
		{
		case BigBoi:
			// katBoi.IncreaseScaleFactor(stat);
			break;
		case LongBoi:
			// katBoi.IncreaseArmLength(stat);
			break;
		case VortexBoi:
			// katBoi.IncreaseRotationSpeed(stat);
			break;
		}
	}

	void Powerup::SetScaleIncrease(std::float_t scaleIncrease)
	{
		mScaleIncrease = scaleIncrease;
	}

	void Powerup::SetLengthIncrease(std::float_t lengthIncrease)
	{
		mLengthIncrease = lengthIncrease;
	}

	void Powerup::SetRotationSpeed(std::float_t rotationSpeed)
	{
		mRotationSpeed = rotationSpeed;
	}

	void Powerup::SetSpawnLocation(glm::vec4 spawnLocation)
	{
		mSpawnLocation = spawnLocation;
	}

	std::float_t Powerup::GetScaleIncrease() const
	{
		return mScaleIncrease;
	}

	std::float_t Powerup::GetLengthIncrease() const
	{
		return mLengthIncrease;
	}

	std::float_t Powerup::GetRotationSpeed() const
	{
		return mRotationSpeed;
	}

	glm::vec4 Powerup::GetSpawnLocation() const
	{
		return mSpawnLocation;
	}

	const std::string Powerup::sRigidBodyKey = "rigidbody";
	const std::string Powerup::sBallColliderKey = "ball collider";
	const std::string Powerup::sBallMeshKey = "ball mesh";
}