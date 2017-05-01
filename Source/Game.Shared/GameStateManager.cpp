
#include "pch.h"
#include "GameStateManager.h"
#include "MenuGamepad.h"
#include "Menu.h"
#include "HUD.h"

#include "Event.h"
#include "EventMessageAttributed.h"

#include "ActionResetRound.h"

#include "KatMusic.h"

#include "ActionUpdateScore.h"	// JUSTIN

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

const string GameStateManager::sMatchWonEventSubtype = "match_won";
const string GameStateManager::sRoundResetEventSubtype = "round_reset";
const string GameStateManager::sStartGameEventSubtype = "start_game";

const GameStateManager::Handlers GameStateManager::sHandlers =
{
	{ sRoundResetEventSubtype, &GameStateManager::ResetWorld },
	{ sMatchWonEventSubtype, &GameStateManager::TransitionToMenu },
	{ sStartGameEventSubtype, &GameStateManager::TransitionToGame }
};

GameStateManager::GameStateManager() :
	mWorldInitialized(false)
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

void GameStateManager::ResetWorld()
{
	mWorld->Reset(*mWorldState);
}

void GameStateManager::TransitionToMenu()
{
	StopSectorMusic(mGameSector);

	// some special operations for the state transition
	Datum& entities = mGameSector->Entities();
	for (uint32_t i = 0; i < entities.Size(); ++i)
	{
		Scope& current = entities.Get<Scope&>(i);

		// delete player gamepads
		if (current.Is(Player::TypeIdClass()))
		{
			static_cast<Player&>(current).DeleteGamepad();
		}
	}

	mGameSector->Orphan();
	mMenuSector->SetWorld(*mWorld);
	mWorld->Reset(*mWorldState);
	StartSectorMusic(mMenuSector);
}

void GameStateManager::TransitionToGame()
{
	StopSectorMusic(mMenuSector);

	// some special operations for the state transition
	Datum& entities = mGameSector->Entities();
	for(uint32_t i = 0; i < entities.Size(); ++i)
	{
		Scope& current = entities.Get<Scope&>(i);

		// reset rounds won
		if (current.Is(HUD::TypeIdClass()))
		{
			static_cast<HUD&>(current).ResetRoundsWon();
		}
		else if (current.Is(Player::TypeIdClass()))
		{
			static_cast<Player&>(current).CreateGamePad();
		}
	}

	mMenuSector->Orphan();
	mGameSector->SetWorld(*mWorld);
	if (!mWorldInitialized)
	{
		mWorld->Initialize(*mWorldState);
		mWorldInitialized = true;
	}
	mWorld->Reset(*mWorldState);
	StartSectorMusic(mGameSector);

	Datum& entites = mGameSector->Entities();
	Menu* pauseMenu = nullptr;
	for (uint32_t i = 0; i < entites.Size(); ++i)
	{
		Scope& current = entites.Get<Scope&>(i);
		if (current.Is(Menu::TypeIdClass()))
		{
			pauseMenu = static_cast<Menu*>(&current);
			break;
		}
	}
	if (pauseMenu != nullptr)
	{
		pauseMenu->RemoveQuadFromView();
	}

	// JUSTIN
	EventMessageAttributed args(ActionUpdateScore::sScoreEventSubtype, mWorldState);
	args.AppendAuxiliaryAttribute(ActionUpdateScore::sPlayerIDKey) = 0;

	Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
	mWorld->Enqueue(*e, *mWorldState, 0);
	// JUSTIN
}

void GameStateManager::StopSectorMusic(Sector* sector)
{
	Datum& entities = sector->Entities();
	for(uint32_t i = 0; i < entities.Size(); ++i)
	{
		if(entities.Get<Scope&>(i).Is(KatMusic::TypeIdClass()))
		{
			static_cast<KatMusic&>(entities.Get<Scope&>(i)).Stop();
		}
	}
}

void GameStateManager::StartSectorMusic(Sector* sector)
{
	Datum& entities = sector->Entities();
	for (uint32_t i = 0; i < entities.Size(); ++i)
	{
		if (entities.Get<Scope&>(i).Is(KatMusic::TypeIdClass()))
		{
			static_cast<KatMusic&>(entities.Get<Scope&>(i)).Play();
		}
	}
}
