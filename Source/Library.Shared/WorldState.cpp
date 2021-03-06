#include "pch.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	WorldState::WorldState() :
		mWorld(nullptr), mSector(nullptr), mEntity(nullptr), mAction(nullptr), mIsPaused(false)
	{

	}

	const std::chrono::high_resolution_clock::time_point& WorldState::GetGameTime() const
	{
		return mGameTime.CurrentTime();
	}

	void WorldState::SetGameTime(const std::chrono::high_resolution_clock::time_point& gameTime)
	{
		mGameTime.SetCurrentTime(gameTime);
	}

	float WorldState::DeltaTime() const
	{
		return mGameTime.ElapsedGameTime().count() / 1000.0f;
	}
}