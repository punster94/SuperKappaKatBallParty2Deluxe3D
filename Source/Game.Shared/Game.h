#pragma once

#include "World.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldState.h"
#include "Renderer.h"
#include "Mesh.h"
#include "SFML/Audio.hpp"

namespace KatBall
{
	class Game final
	{
	public:
		Game(FieaGameEngine::Renderer& renderer);

		void Run();

	private:
		void Init();
		void Update();
		void Shutdown();

		FieaGameEngine::Mesh* mMeshTest;
		FieaGameEngine::Renderer* mRenderer;
		sf::Music mBackgroundMusic;
		FieaGameEngine::World mWorld;
		FieaGameEngine::WorldState mWorldState;
		FieaGameEngine::GameTime mGameTime;
		FieaGameEngine::GameClock mGameClock;
	};
}
