#include "pch.h"
#include "Powerup.h"

namespace KatBall
{
	Powerup::Powerup():
		Entity("powerup"), mType(PowerupType::Default), mScaleIncrease(), mLengthIncrease(), mRotationSpeed(), mMeshEntity(nullptr), mRigidBody(nullptr)
	{
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
	}

	void Powerup::Initialize(FieaGameEngine::WorldState& worldState)
	{
		Entity::Initialize(worldState);

		mMeshEntity = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();
		mRigidBody = FindChildEntityByName(sBallColliderKey)->As<RigidBody>();
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

	void Powerup::SetType(PowerupType type)
	{
		mType = type;
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

	Powerup::PowerupType Powerup::GetType() const
	{
		return mType;
	}

	void Powerup::CopyPrivateDataMembers(const Powerup& otherPowerup)
	{
		mRigidBody = otherPowerup.mRigidBody;
		mMeshEntity = otherPowerup.mMeshEntity;

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
	}

	const std::string Powerup::sRigidBodyKey = "rigidbody";
	const std::string Powerup::sBallColliderKey = "ball collider";
	const std::string Powerup::sBallMeshKey = "ball mesh";

	const std::string Powerup::sScaleIncreaseKey = "scale increase";
	const std::string Powerup::sLengthIncreaseKey = "length increase";
	const std::string Powerup::sRotationSpeedKey = "rotation speed";
}