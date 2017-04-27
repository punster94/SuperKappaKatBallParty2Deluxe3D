#pragma once

#include "World.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldState.h"
#include "Renderer.h"
#include "KatMusic.h"

namespace KatBall
{
	class Game final
	{
	public:

		explicit Game(FieaGameEngine::Renderer& renderer);

		void Run();

	private:
		void Init();
		void Update();
		void Shutdown();

		void DebugUpdate();

		void LoadAssets();

		FieaGameEngine::Renderer* mRenderer;
		FieaGameEngine::KatMusic mBackgroundMusic;
		FieaGameEngine::World mWorld;
		FieaGameEngine::WorldState mWorldState;
		FieaGameEngine::GameTime mGameTime;
		FieaGameEngine::GameClock mGameClock;
	};
}
