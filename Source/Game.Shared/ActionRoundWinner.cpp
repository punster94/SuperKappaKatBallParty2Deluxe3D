
#include "pch.h"
#include "ActionRoundWinner.h"

#include "EventMessageAttributed.h"
#include "Event.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionRoundWinner)

const string ActionRoundWinner::sMatchWonEventSubtype = "match_won";
const string ActionRoundWinner::sMatchWinnerIDKey = "match_winner";

const uint32_t ActionRoundWinner::sNoMatchWinnerFlag = -1;
const uint32_t ActionRoundWinner::sTieGameFlag = NUM_PLAYERS;

ActionRoundWinner::ActionRoundWinner(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ActionRoundWinner::~ActionRoundWinner()
{
}

void ActionRoundWinner::Update(class WorldState& worldState)
{
	Scope* scope = nullptr;
	if(Datum* scoresDatum = Search(HUD::sScoresKey, &scope))
	{
		// get high score
		uint32_t highScore = 0;
		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(score->GetScore() >= highScore)
			{
				highScore = score->GetScore();
			}
		}

		// update players that have the high score
		int32_t matchWinnerID = sNoMatchWinnerFlag;
		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			// score matches high score -- round win for current player
			Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(score->GetScore() == highScore)
			{
				score->UpdateNumWins();
			}

			// does the current player have enough round wins for a mtach win?
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

		// reset if no match winner
		if(matchWinnerID == sNoMatchWinnerFlag)
		{
			if(HUD* hud = scope->As<HUD>())
			{
				// TODO -- reset everything for round
				hud->Reset();
			}
		}

		// else post match won event
		else
		{
			EventMessageAttributed args(sMatchWonEventSubtype, &worldState);
			args.AppendAuxiliaryAttribute(sMatchWinnerIDKey) = matchWinnerID;

			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
			worldState.mWorld->Enqueue(*e, worldState, 0);
		}
	}
}
