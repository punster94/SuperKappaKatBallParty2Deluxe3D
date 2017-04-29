#include "pch.h"
#include "PowerupSpawner.h"
#include <random>


namespace KatBall
{
	PowerupSpawner::PowerupSpawner():
		mSpawnLocation(glm::vec4(0)), mSpawnChance(0.0f)
	{
		(*this)["SpawnLocation"].SetStorage(&mSpawnLocation, 1);
		(*this)["SpawnChance"].SetStorage(&mSpawnChance, 1);
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

	void PowerupSpawner::SetSpawnChance(const float spawnChance)
	{
		mSpawnChance = spawnChance;
	}

	void PowerupSpawner::AttemptSpawn()
	{
		std::default_random_engine generator;
		std::uniform_int_distribution<std::uint32_t> distribution(0, 100);
		std::uint32_t roll = distribution(generator);

		if (roll <= mSpawnChance)
		{
			// TODO: Pick a random powerup to spawn based on spawn weights, at mSpawnLocation
		}
	}
}
