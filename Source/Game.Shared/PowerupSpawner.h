#pragma once
#include "Entity.h"
#include <random>
#include "KatSound.h"

namespace KatBall
{
	class PowerupSpawner : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(PowerupSpawner, FieaGameEngine::Entity)

	public:
		/// Constructor
		PowerupSpawner();

		/// Destructor (defaulted)
		~PowerupSpawner() = default;

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;

		virtual void Update(FieaGameEngine::WorldState& worldState) override;

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

		/// Accessor method for the long Boi's length increase stat
		/// @Return: The amount that long Boi increases length by
		float GetLongBoiLengthIncrease() const;

		/// Mutator method for the long Boi's length increase stat
		/// @Param lengthIncrease: The amount that long Boi increases length by
		void SetLongBoiLengthIncrease(const float& lengthIncrease);

		/// Accessor method for the big Boi's scale increase stat
		/// @Return: The amount that big Boi increases the player's scale by
		float GetBigBoiScaleIncrease() const;

		/// Mutator method for the big Boi's scale increase stat
		/// @Param scaleIncrease: The amount that big Boi increases the player's scale by
		void SetBigBoiScaleIncrase(const float& scaleIncrease);

		/// Accessor method for the vortex Boi rotation speed stat
		/// @Return: The rotation speed increase for vortex Boi
		float getVortexBoiRotationSpeed() const;

		/// Mutator method for the vortex Boi rotation speed stat
		/// @Param rotationSpeed: The rotation speed increase for vortex Boi
		void SetVortexBoiRotationSpeed(const float& rotationSpeed);

		/// Accessor method for long Boi spawn weight
		/// @Return: Spawn weight of long Boi
		std::int32_t GetLongBoiSpawnWeight() const;

		/// Mutator method for long Boi spawn weight
		/// @Param spawnWeight: Spawn weight of long Boi
		void SetLongBoiSpawnWeight(const std::int32_t& spawnWeight);

		/// Accessor method for big Boi spawn weight
		/// @Return: Spawn weight of big Boi
		std::int32_t GetBigBoiSpawnWeight() const;

		/// Mutator method for big Boi spawn weight
		/// @Param spawnWeight: Spawn weight of big Boi
		void SetBigBoiSpawnWeight(const std::int32_t& spawnWeight);

		/// Accessor method for vortex Boi spawn weight
		/// @Return: Spawn weight for vortex Boi
		std::int32_t GetVortexBoiSpawnWeight() const;

		/// Mutator method for vortex Boi spawn weight
		/// @Param spawnWeight: Spawn weight for vortex Boi
		void SetVortexBoiSpawnWeight(const std::int32_t& spawnWeight);

		/// Rolls for chance to spawn, and on success, instantiates a powerup at the spawn location
		/// Rolls are between 1 and 100. If roll is <= spawnChance, a powerup is spawned
		void AttemptSpawn();

	private:
		RigidBody* mRigidBody;
		MeshEntity* mMeshEntity;
		FieaGameEngine::KatSound* mSpawnSound;
		glm::vec4 mSpawnLocation;	// Location that powerups will be spawned from this object
		float mSpawnChance;			// Chance between 0.0 and 100.0 that this spawner will generate an item
		float mElapsedTime;

		std::int32_t mLongBoiSpawnWeight;	// Relative chance for spawning long Bois
		std::int32_t mBigBoiSpawnWeight;	// Relative chance for spawning big Bois
		std::int32_t mVortexBoiSpawnWeight;	// Relative chance for spawning vortex Bois

		float mLongBoiLengthIncrease;	// The length increase of longBoi on pickup
		float mBigBoiScaleIncrease;		// The size scale incrrase of bigBoi on pickup
		float mVortexBoiRotationSpeed;	// How fast the vortex Boi goes whoosh

		const float mSpawnAttemptInterval = 5.0f;

		static const std::string sRigidBodyKey;
		static const std::string sBallColliderKey;
		static const std::string sBallMeshKey;
		static const std::string sSpawnSoundKey;
		std::default_random_engine mGenerator;
	};

	ConcreteEntityFactory(PowerupSpawner);
}
