#pragma once

#include "World.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldState.h"
#include "Renderer.h"

#include "GameStateManager.h"

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
		FieaGameEngine::World mWorld;
		FieaGameEngine::Sector* mMenuSector;
		FieaGameEngine::Sector* mGameSector;
		FieaGameEngine::WorldState mWorldState;
		FieaGameEngine::GameTime mGameTime;
		FieaGameEngine::GameClock mGameClock;

		GameStateManager mStateManager;

	public:

		// TODO -- move this to menu object???
		static const std::string sStartGameEventSubtype;
	};
}
