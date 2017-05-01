
#include "pch.h"
#include "ActionResetRound.h"

#include "HUD.h"
#include "Score.h"

#include "EventMessageAttributed.h"
#include "Event.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionResetRound)

const string ActionResetRound::sMatchWonEventSubtype = "match_won";
const string ActionResetRound::sMatchWinnerIDKey = "match_winner";

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
		int32_t matchWinnerID = sNoMatchWinnerFlag;
		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(score->GetNumWins() == ROUNDS_TO_WIN)
			{
				// tie game if the match winner ID has already been set
				if(matchWinnerID != sNoMatchWinnerFlag)
				{
					matchWinnerID = sTieGameFlag;
					break;
				}

				matchWinnerID = i;
			}
		}

		// no match winner -- reset round
		if(matchWinnerID == sNoMatchWinnerFlag)
		{
			// TODO -- post reset round event

			Scope* scope = nullptr;
			Search(HUD::sScoresKey, &scope);
			if(HUD* hud = scope->As<HUD>())
			{
				hud->Reset();
			}
		}

		// match winner(s) -- load menu
		else
		{
			EventMessageAttributed args(sMatchWonEventSubtype, &worldState);
			args.AppendAuxiliaryAttribute(sMatchWinnerIDKey) = matchWinnerID;

			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
			worldState.mWorld->Enqueue(*e, worldState, 0);
		}
	}
}
