#include "pch.h"

namespace FieaGameEngine
{
	WorldState::WorldState() :
		mWorld(nullptr), mSector(nullptr), mEntity(nullptr), mAction(nullptr)
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
}