#include "pch.h"
#include "PowerupSpawner.h"
#include <random>


namespace KatBall
{
	PowerupSpawner::PowerupSpawner() :
		mSpawnLocation(glm::vec4(0)), mSpawnChance(0.0f)
	{
		PowerupSpawner::InitializeSignatures();
	}

	void PowerupSpawner::InitializeSignatures()
	{
		Entity::InitializeSignatures();
		AddExternalAttribute("Spawn Location", &mSpawnLocation, 1);
		AddExternalAttribute("Spawn Chance", &mSpawnChance, 1);

		AddExternalAttribute("Long Boy Spawn Weight", &mLongBoySpawnWeight, 1);
		AddExternalAttribute("Big Boy Spawn Weight", &mBigBoySpawnWeight, 1);
		AddExternalAttribute("Vortex Boy Spawn Weight", &mVortexBoySpawnWeight, 1);

		AddExternalAttribute("Long Boy Length Increase", &mLongBoyLengthIncrease, 1);
		AddExternalAttribute("Big Boy Scale Increase", &mBigBoyScaleIncrease, 1);
		AddExternalAttribute("Vortex Boy Rotation Speed", &mVortexBoyRotationSpeed, 1);
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

	float PowerupSpawner::GetLongBoyLengthIncrease() const
	{
		return mLongBoyLengthIncrease;
	}

	void PowerupSpawner::SetLongBoyLengthIncrease(const float& lengthIncrease)
	{
		mLongBoyLengthIncrease = lengthIncrease;
	}

	float PowerupSpawner::GetBigBoyScaleIncrease() const
	{
		return mBigBoyScaleIncrease;
	}

	void PowerupSpawner::SetBigBoyScaleIncrase(const float& scaleIncrease)
	{
		mBigBoyScaleIncrease = scaleIncrease;
	}

	float PowerupSpawner::getVortexBoyRotationSpeed() const
	{
		return mVortexBoyRotationSpeed;
	}

	void PowerupSpawner::SetVortexBoyRotationSpeed(const float& rotationSpeed)
	{
		mVortexBoyRotationSpeed = rotationSpeed;
	}

	std::int32_t PowerupSpawner::GetLongBoySpawnWeight() const
	{
		return mLongBoySpawnWeight;
	}

	void PowerupSpawner::SetLongBoySpawnWeight(const std::int32_t& spawnWeight)
	{
		mLongBoySpawnWeight = spawnWeight;
	}

	std::int32_t PowerupSpawner::GetBigBoySpawnWeight() const
	{
		return mBigBoySpawnWeight;
	}

	void PowerupSpawner::SetBigBoySpawnWeight(const std::int32_t& spawnWeight)
	{
		mBigBoySpawnWeight = spawnWeight;
	}

	std::int32_t PowerupSpawner::GetVortexBoySpawnWeight() const
	{
		return mVortexBoySpawnWeight;
	}

	void PowerupSpawner::SetVortexBoySpawnWeight(const std::int32_t& spawnWeight)
	{
		mVortexBoySpawnWeight = spawnWeight;
	}

	void PowerupSpawner::AttemptSpawn()
	{
		std::default_random_engine generator;
		std::uniform_int_distribution<std::uint32_t> distribution(0, 100);
		std::uint32_t roll = distribution(generator);

		if (roll <= mSpawnChance)
		{
			std::uint32_t totalSpawnWeight = mLongBoySpawnWeight + mBigBoySpawnWeight + mVortexBoySpawnWeight;
			std::uniform_int_distribution<std::uint32_t> weightedDistribution(0, totalSpawnWeight);
			std::uint32_t weightedRoll = distribution(generator);

			if (weightedRoll <= mLongBoySpawnWeight)
			{
				// TODO: Spawn a long boy
			}
			else if (weightedRoll <= mLongBoySpawnWeight + mBigBoySpawnWeight)
			{
				// TODO: Spawn a big boy
			}
			else if (weightedRoll <= mLongBoySpawnWeight + mBigBoySpawnWeight + mVortexBoySpawnWeight)
			{
				// TODO: Spawn a vortex boy
			}
		}
	}
}
