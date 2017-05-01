#include "pch.h"
#include "Powerup.h"

namespace KatBall
{
	Powerup::Powerup(PowerupType type, std::float_t stat, glm::vec4 location) :
		Entity("powerup"), mType(type), mSpawnLocation(location), mScaleIncrease(0), mLengthIncrease(0), mRotationSpeed(0), mMeshEntity(nullptr), mRigidBody(nullptr)
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

	Powerup::Powerup(const Powerup& otherPowerup):
		Entity(otherPowerup)
	{
		CopyPrivateDataMembers(otherPowerup);
	}

	void Powerup::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute(sScaleIncreaseKey, &mScaleIncrease, 1);
		AddExternalAttribute(sLengthIncreaseKey, &mLengthIncrease, 1);
		AddExternalAttribute(sRotationSpeedKey, &mRotationSpeed, 1);
		AddExternalAttribute(sSpawnLocationKey, &mSpawnLocation, 1);
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
			// katBoi.IncreaseScaleFactor(mScaleIncrease);
			break;
		case LongBoi:
			// katBoi.IncreaseArmLength(mLengthIncrease);
			break;
		case VortexBoi:
			// katBoi.IncreaseRotationSpeed(mRotationSpeed);
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

	void Powerup::CopyPrivateDataMembers(const Powerup& otherPowerup)
	{
		mRigidBody = otherPowerup.mRigidBody;
		mMeshEntity = otherPowerup.mMeshEntity;
		mSpawnLocation = otherPowerup.mSpawnLocation;

		mType = otherPowerup.mType;
		mScaleIncrease = otherPowerup.mScaleIncrease;
		mLengthIncrease = otherPowerup.mLengthIncrease;
		mRotationSpeed = otherPowerup.mRotationSpeed;


		FixExternalAttributes();
	}

	void Powerup::FixExternalAttributes()
	{
		Append(sScaleIncreaseKey).SetStorage(&mScaleIncrease, 1);
		Append(sLengthIncreaseKey).SetStorage(&mLengthIncrease, 1);
		Append(sRotationSpeedKey).SetStorage(&mRotationSpeed, 1);
		Append(sSpawnLocationKey).SetStorage(&mSpawnLocation, 1);
	}

	const std::string Powerup::sRigidBodyKey = "rigidbody";
	const std::string Powerup::sBallColliderKey = "ball collider";
	const std::string Powerup::sBallMeshKey = "ball mesh";

	const std::string Powerup::sScaleIncreaseKey = "scale increase";
	const std::string Powerup::sLengthIncreaseKey = "length increase";
	const std::string Powerup::sRotationSpeedKey = "rotation speed";
	const std::string Powerup::sSpawnLocationKey = "spawn location";
}