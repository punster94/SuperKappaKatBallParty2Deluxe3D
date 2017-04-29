#pragma once
#include "Entity.h"

namespace KatBall
{
	class PowerupSpawner : public FieaGameEngine::Entity
	{
	public:

		PowerupSpawner();
		
		~PowerupSpawner();

	private:
		glm::vec4 mLocation;
		float mSpawnChance;
	};
}

