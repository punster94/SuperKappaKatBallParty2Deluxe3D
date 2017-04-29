#include "pch.h"
#include "PowerupSpawner.h"


namespace KatBall
{
	PowerupSpawner::PowerupSpawner()
	{
		(*this)["Location"].SetStorage(&mLocation, 1);
		(*this)["SpawnChance"].SetStorage(&mSpawnChance, 1);
	}

	PowerupSpawner::~PowerupSpawner()
	{
	}
}