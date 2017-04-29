#pragma once

#include "Entity.h"
#include "Kat.h"

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

		/** Powerup collection
		 *	Applies the powerup type effect on the provided kat.
		 *	@param katBoi the kat that has collected this powerup.
		 */
		void OnCollect(Kat& katBoi);

		void SetScaleIncrease(std::float_t scaleIncrease);
		void SetLengthIncrease(std::float_t lengthIncrease);
		void SetRotationSpeed(std::float_t rotationSpeed);

		std::float_t GetScaleIncrease() const;
		std::float_t GetLengthIncrease() const;
		std::float_t GetRotationSpeed() const;

	private:
		void InitializeSignatures();

		const std::string mName;
		const std::string mScaleIncreaseKey;
		const std::string mLengthIncreaseKey;
		const std::string mRotationSpeedKey;

		std::float_t mScaleIncrease;
		std::float_t mLengthIncrease;
		std::float_t mRotationSpeed;
		glm::vec4 mLocation;

		PowerupType mType;
	};
}