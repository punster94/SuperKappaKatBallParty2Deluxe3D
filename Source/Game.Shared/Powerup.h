#pragma once

#include "Entity.h"
#include "Kat.h"

using namespace FieaGameEngine;

namespace KatBall
{
	class Powerup final : public Entity
	{
	public:
		enum PowerupType
		{
			BigBoi,
			LongBoi,
			VortexBoi
		};

		Powerup(PowerupType type, std::float_t stat, glm::vec4 location);
		void InitializeSignatures() override;
		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;

		/** Powerup collection
		 *	Applies the powerup type effect on the provided kat.
		 *	@param katBoi the player that has collected this powerup.
		 */
		void OnCollect(Player& katBoi);

		void SetScaleIncrease(std::float_t scaleIncrease);
		void SetLengthIncrease(std::float_t lengthIncrease);
		void SetRotationSpeed(std::float_t rotationSpeed);
		void SetSpawnLocation(glm::vec4 spawnLocation);

		std::float_t GetScaleIncrease() const;
		std::float_t GetLengthIncrease() const;
		std::float_t GetRotationSpeed() const;
		glm::vec4 GetSpawnLocation() const;

	private:
		RigidBody* mRigidBody;
		MeshEntity* mMeshEntity;
		glm::vec4 mSpawnLocation;

		std::float_t mScaleIncrease;
		std::float_t mLengthIncrease;
		std::float_t mRotationSpeed;

		PowerupType mType;

		const std::string mName = "powerup";
		const std::string mScaleIncreaseKey = "scale increase";
		const std::string mLengthIncreaseKey = "length increase";
		const std::string mRotationSpeedKey = "rotation speed";
		const std::string mSpawnLocationKey = "spawn location";

		static const std::string sRigidBodyKey;
		static const std::string sBallColliderKey;
		static const std::string sBallMeshKey;
	};
}