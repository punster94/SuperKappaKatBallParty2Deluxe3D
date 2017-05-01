#include "pch.h"
#include "PowerupSpawner.h"
#include <random>
#include "Powerup.h"


namespace KatBall
{
	PowerupSpawner::PowerupSpawner() :
		mSpawnLocation(glm::vec4(mPosition.x, mPosition.y, mPosition.z, 0)), mSpawnChance(0.0f), mElapsedTime(0.0f), mLongBoiLengthIncrease(0.0f), mBigBoiScaleIncrease(0.0f), mVortexBoiRotationSpeed(0.0f)
	{
		PowerupSpawner::InitializeSignatures();
		unsigned randomSeed = std::chrono::system_clock::now().time_since_epoch().count();
		mGenerator.seed(randomSeed);
	}

	void PowerupSpawner::Initialize(FieaGameEngine::WorldState& worldState)
	{
		Entity::Initialize(worldState);
		mMeshEntity = FindChildEntityByName(sBallMeshKey)->As<MeshEntity>();
	}

	void PowerupSpawner::Update(FieaGameEngine::WorldState& worldState)
	{
		mElapsedTime += worldState.DeltaTime();
		if (mElapsedTime >= mSpawnAttemptInterval)
		{	// Every 5 seconds, attempt to spawn this thing
			mElapsedTime = 0.0f;
			AttemptSpawn();
		}
	}

	void PowerupSpawner::InitializeSignatures()
	{
		Entity::InitializeSignatures();

		AddExternalAttribute("Spawn Location", &mSpawnLocation, 1);
		AddExternalAttribute("Spawn Chance", &mSpawnChance, 1);

		AddExternalAttribute("Long Boi Spawn Weight", &mLongBoiSpawnWeight, 1);
		AddExternalAttribute("Big Boi Spawn Weight", &mBigBoiSpawnWeight, 1);
		AddExternalAttribute("Vortex Boi Spawn Weight", &mVortexBoiSpawnWeight, 1);

		AddExternalAttribute("Long Boi Length Increase", &mLongBoiLengthIncrease, 1);
		AddExternalAttribute("Big Boi Scale Increase", &mBigBoiScaleIncrease, 1);
		AddExternalAttribute("Vortex Boi Rotation Speed", &mVortexBoiRotationSpeed, 1);
	}

	glm::vec4 PowerupSpawner::GetSpawnLocation() const
	{
		return mSpawnLocation;
	}

	void PowerupSpawner::SetSpawnLocation(const glm::vec4& spawnLocation)
	{
		mSpawnLocation = spawnLocation;
	}

	float PowerupSpawner::GetSpawnChance() const
	{
		return mSpawnChance;
	}

	void PowerupSpawner::SetSpawnChance(const float& spawnChance)
	{
		mSpawnChance = spawnChance;
	}

	float PowerupSpawner::GetLongBoiLengthIncrease() const
	{
		return mLongBoiLengthIncrease;
	}

	void PowerupSpawner::SetLongBoiLengthIncrease(const float& lengthIncrease)
	{
		mLongBoiLengthIncrease = lengthIncrease;
	}

	float PowerupSpawner::GetBigBoiScaleIncrease() const
	{
		return mBigBoiScaleIncrease;
	}

	void PowerupSpawner::SetBigBoiScaleIncrase(const float& scaleIncrease)
	{
		mBigBoiScaleIncrease = scaleIncrease;
	}

	float PowerupSpawner::getVortexBoiRotationSpeed() const
	{
		return mVortexBoiRotationSpeed;
	}

	void PowerupSpawner::SetVortexBoiRotationSpeed(const float& rotationSpeed)
	{
		mVortexBoiRotationSpeed = rotationSpeed;
	}

	std::int32_t PowerupSpawner::GetLongBoiSpawnWeight() const
	{
		return mLongBoiSpawnWeight;
	}

	void PowerupSpawner::SetLongBoiSpawnWeight(const std::int32_t& spawnWeight)
	{
		mLongBoiSpawnWeight = spawnWeight;
	}

	std::int32_t PowerupSpawner::GetBigBoiSpawnWeight() const
	{
		return mBigBoiSpawnWeight;
	}

	void PowerupSpawner::SetBigBoiSpawnWeight(const std::int32_t& spawnWeight)
	{
		mBigBoiSpawnWeight = spawnWeight;
	}

	std::int32_t PowerupSpawner::GetVortexBoiSpawnWeight() const
	{
		return mVortexBoiSpawnWeight;
	}

	void PowerupSpawner::SetVortexBoiSpawnWeight(const std::int32_t& spawnWeight)
	{
		mVortexBoiSpawnWeight = spawnWeight;
	}

	void PowerupSpawner::AttemptSpawn()
	{
		std::uniform_int_distribution<std::uint32_t> distribution(0, 100);
		std::uint32_t roll = distribution(mGenerator);

		if (roll <= mSpawnChance)
		{
			mScale.x = (mScale.x == 2) ? 0.5 : 2;
			mScale.y = (mScale.y == 2) ? 0.5 : 2;
			mScale.z = (mScale.z == 2) ? 0.5 : 2;
			std::uint32_t totalSpawnWeight = mLongBoiSpawnWeight + mBigBoiSpawnWeight + mVortexBoiSpawnWeight;
			std::uniform_int_distribution<std::uint32_t> weightedDistribution(0, totalSpawnWeight);
			std::uint32_t weightedRoll = distribution(mGenerator);

			if (weightedRoll <= mLongBoiSpawnWeight)
			{
				(*this)[FieaGameEngine::Sector::sSectorEntitiesKey].PushBack(*new Powerup(Powerup::PowerupType::LongBoi, mLongBoiLengthIncrease, mSpawnLocation));
			}
			else if (weightedRoll <= mLongBoiSpawnWeight + mBigBoiSpawnWeight)
			{
				(*this)[FieaGameEngine::Sector::sSectorEntitiesKey].PushBack(*new Powerup(Powerup::PowerupType::BigBoi, mBigBoiScaleIncrease, mSpawnLocation));
			}
			else if (weightedRoll <= mLongBoiSpawnWeight + mBigBoiSpawnWeight + mVortexBoiSpawnWeight)
			{
				(*this)[FieaGameEngine::Sector::sSectorEntitiesKey].PushBack(*new Powerup(Powerup::PowerupType::VortexBoi, mVortexBoiRotationSpeed, mSpawnLocation));
			}
		}
	}

	const std::string PowerupSpawner::sRigidBodyKey = "rigidbody";
	const std::string PowerupSpawner::sBallColliderKey = "ball collider";
	const std::string PowerupSpawner::sBallMeshKey = "ball mesh";
}
