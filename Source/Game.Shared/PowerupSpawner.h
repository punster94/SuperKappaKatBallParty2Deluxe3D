#pragma once
#include "Entity.h"

namespace KatBall
{
	class PowerupSpawner : public FieaGameEngine::Entity
	{
	public:

		/// Constructor
		PowerupSpawner();
		
		/// Destructor (defaulted)
		~PowerupSpawner() = default;

		/// Accessor method for the powerup spawner's spawn location
		/// @Return: The location that the powerups from this object will spawn
		glm::vec4 GetSpawnLocation() const;

		/// Mutator method for the powerup spawner's spawn location
		/// @Param spawnLocation: The new location from which powerups will spawn from this object
		void SetSpawnLocation(const glm::vec4& spawnLocation);

		/// Accessor method for the powerup spawner's spawn chance
		/// @Return: The chance that this object will spawn a powerup
		float GetSpawnChance() const;

		/// Mutator method for the powerup spawner's spawn chance
		/// @Param spawnChance: The chance that this object will spawn a powerup
		void SetSpawnChance(const float spawnChance);

		/// Rolls for chance to spawn, and on success, instantiates a powerup at the spawn location
		/// Rolls are between 1 and 100. If roll is <= spawnChance, a powerup is spawned
		void AttemptSpawn();

	private:
		glm::vec4 mSpawnLocation; // Location that powerups will be spawned from this object
		float mSpawnChance; // Chance between 0.0 and 100.0 that this spawner will generate an item
	};
}

