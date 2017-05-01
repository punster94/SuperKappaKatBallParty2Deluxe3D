
#include "pch.h"
#include "ActionResetRound.h"
#include "GameStateManager.h"

#include "HUD.h"
#include "Score.h"

#include "EventMessageAttributed.h"
#include "Event.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionResetRound)

const string ActionResetRound::sMatchWinnersKey = "match_winners";

const uint32_t ActionResetRound::sNoMatchWinnerFlag = -1;
const uint32_t ActionResetRound::sTieGameFlag = NUM_PLAYERS;

ActionResetRound::ActionResetRound(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ActionResetRound::~ActionResetRound()
{
}

void ActionResetRound::Update(class WorldState& worldState)
{
	if(Datum* scoresDatum = Search(HUD::sScoresKey))
	{
		// find match winners, if any
		Datum matchWinners;
		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(score->GetNumWins() == ROUNDS_TO_WIN)
			{
				matchWinners.PushBack(static_cast<int32_t>(i));
			}
		}

		// no match winner -- reset round
		if(matchWinners.Size() == 0)
		{
			EventMessageAttributed args(GameStateManager::sRoundResetEventSubtype, &worldState);
			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
			worldState.mWorld->Enqueue(*e, worldState, 0);
		}

		// match winner(s) -- load menu
		else
		{
			EventMessageAttributed args(GameStateManager::sMatchWonEventSubtype, &worldState);
			args.AppendAuxiliaryAttribute(sMatchWinnersKey) = matchWinners;

			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
			worldState.mWorld->Enqueue(*e, worldState, 0);
		}
	}
}
