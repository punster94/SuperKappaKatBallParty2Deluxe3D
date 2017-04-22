#pragma once

#include "World.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldState.h"
#include "Renderer.h"

namespace KatBall
{
	class Game
	{
	public:
		void Run();

	private:
		void Init();
		void Update();

		FieaGameEngine::Renderer* mRenderer;
		FieaGameEngine::World mWorld;
		FieaGameEngine::WorldState mWorldState;
		FieaGameEngine::GameTime mGameTime;
		FieaGameEngine::GameClock mGameClock;
	};
}