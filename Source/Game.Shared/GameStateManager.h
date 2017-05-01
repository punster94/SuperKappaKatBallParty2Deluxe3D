
#pragma once

#include "World.h"
#include "EventSubscriber.h"

#include "HashMap.h"

namespace KatBall
{
	class GameStateManager final : public FieaGameEngine::EventSubscriber
	{
	public:

		GameStateManager();
		~GameStateManager();

		void Initialize(FieaGameEngine::World& world, FieaGameEngine::Sector& menu, FieaGameEngine::Sector& game, FieaGameEngine::WorldState& worldState);
		virtual void Notify(const FieaGameEngine::EventPublisher& eventPublisher) override;

	private:

		void ResetWorld();
		void TransitionToMenu();
		void TransitionToGame();

		void StopSectorMusic(FieaGameEngine::Sector* sector);

		FieaGameEngine::World* mWorld;
		FieaGameEngine::Sector* mMenuSector;
		FieaGameEngine::Sector* mGameSector;
		FieaGameEngine::WorldState* mWorldState;


		typedef std::function<void(GameStateManager&)> HandlerFunc;
		typedef FieaGameEngine::HashMap<std::string, HandlerFunc> Handlers;

		static const Handlers sHandlers;
	};
}
