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

		/// Initializes prescribed attributes for this class
		void InitializeSignatures() override;

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
		void SetSpawnChance(const float& spawnChance);

		/// Accessor method for the long boy's length increase stat
		/// @Return: The amount that long boy increases length by
		float GetLongBoyLengthIncrease() const;

		/// Mutator method for the long boy's length increase stat
		/// @Param lengthIncrease: The amount that long boy increases length by
		void SetLongBoyLengthIncrease(const float& lengthIncrease);

		/// Accessor method for the big boy's scale increase stat
		/// @Return: The amount that big boy increases the player's scale by
		float GetBigBoyScaleIncrease() const;

		/// Mutator method for the big boy's scale increase stat
		/// @Param scaleIncrease: The amount that big boy increases the player's scale by
		void SetBigBoyScaleIncrase(const float& scaleIncrease);

		/// Accessor method for the vortex boy rotation speed stat
		/// @Return: The rotation speed increase for vortex boy
		float getVortexBoyRotationSpeed() const;

		/// Mutator method for the vortex boy rotation speed stat
		/// @Param rotationSpeed: The rotation speed increase for vortex boy
		void SetVortexBoyRotationSpeed(const float& rotationSpeed);

		std::int32_t GetLongBoySpawnWeight() const;
		void SetLongBoySpawnWeight(const std::int32_t& spawnWeight);

		std::int32_t GetBigBoySpawnWeight() const;
		void SetBigBoySpawnWeight(const std::int32_t& spawnWeight);

		std::int32_t GetVortexBoySpawnWeight() const;
		void SetVortexBoySpawnWeight(const std::int32_t& spawnWeight);

		/// Rolls for chance to spawn, and on success, instantiates a powerup at the spawn location
		/// Rolls are between 1 and 100. If roll is <= spawnChance, a powerup is spawned
		void AttemptSpawn();

	private:
		glm::vec4 mSpawnLocation; // Location that powerups will be spawned from this object
		float mSpawnChance; // Chance between 0.0 and 100.0 that this spawner will generate an item

		std::int32_t mLongBoySpawnWeight;	// Relative chance for spawning long boys
		std::int32_t mBigBoySpawnWeight;	// Relative chance for spawning big boys
		std::int32_t mVortexBoySpawnWeight;	// Relative chance for spawning vortex boys

		float mLongBoyLengthIncrease; // The length increase of longboy on pickup
		float mBigBoyScaleIncrease; // The size scale incrrase of bigboy on pickup
		float mVortexBoyRotationSpeed; // How fast the vortex boy goes whoosh
	};
}

