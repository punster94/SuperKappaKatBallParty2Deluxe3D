
#include "pch.h"
#include "GameStateManager.h"
#include "Game.h"

#include "Event.h"
#include "EventMessageAttributed.h"

#include "ActionRoundWinner.h"

#include "KatMusic.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

GameStateManager::GameStateManager()
{
	Event<EventMessageAttributed>::Subscribe(*this);
}

GameStateManager::~GameStateManager()
{
	Event<EventMessageAttributed>::Unsubscribe(*this);
}

void GameStateManager::Initialize(World& world, Sector& menu, Sector& game, WorldState& worldState)
{
	// doesn't manage any of this memory, just holds it
	mWorld = &world;
	mMenuSector = &menu;
	mGameSector = &game;
	mWorldState = &worldState;
}

void GameStateManager::Notify(const EventPublisher& eventPublisher)
{
	assert(mWorld != nullptr);
	assert(mMenuSector != nullptr);
	assert(mGameSector != nullptr);
	assert(mWorldState != nullptr);

	Event<EventMessageAttributed>* e = eventPublisher.As<Event<EventMessageAttributed>>();
	if(e != nullptr)
	{
		const EventMessageAttributed& args = e->Message();
		if(sHandlers.ContainsKey(args.GetSubtype()))
		{
			sHandlers[args.GetSubtype()](*this);
		}
	}
}

void GameStateManager::TransitionToMenu()
{
	StopSectorMusic(mGameSector);

	mGameSector->Orphan();
	mMenuSector->SetWorld(*mWorld);
	mWorld->Initialize(*mWorldState);
}

void GameStateManager::TransitionToGame()
{
	StopSectorMusic(mMenuSector);

	mMenuSector->Orphan();
	mGameSector->SetWorld(*mWorld);
	mWorld->Initialize(*mWorldState);
}

void GameStateManager::StopSectorMusic(Sector* sector)
{
	Datum& entities = sector->Entities();
	for(uint32_t i = 0; i < entities.Size(); ++i)
	{
		if(entities.Get<Scope&>(i).Is(KatMusic::TypeIdClass()))
		{
			static_cast<KatMusic&>(entities.Get<Scope&>(i)).Stop();
			break;
		}
	}
}

const GameStateManager::Handlers GameStateManager::sHandlers =
{
	{ ActionRoundWinner::sMatchWonEventSubtype, &GameStateManager::TransitionToMenu },
	{ Game::sStartGameEventSubtype, &GameStateManager::TransitionToGame }
};
