
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
const string ActionRoundWinner::sMatchWinnerKey = "match_winner";

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
	if(Datum* scoresDatum = Search(HUD::sScoresKey))
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

		// update players with high score
		int32_t matchWinner = -1;
		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(score->GetScore() == highScore)
			{
				score->UpdateNumWins();
			}

			// find match winners
			if(score->GetNumWins() == ROUNDS_TO_WIN)
			{
				// tie game if match winner was already set
				if(matchWinner != -1)
				{
					matchWinner = NUM_PLAYERS;
					break;
				}

				matchWinner = i;
			}
			
			score->Reset();
		}

		Datum* timerDatum = Search(HUD::sTimerKey);
		Timer* timer = static_cast<Timer*>(timerDatum->Get<RTTI*&>());
		timer->Reset();

		// match winner declared -- post event
		if(matchWinner != -1)
		{
			EventMessageAttributed args(sMatchWonEventSubtype, &worldState);
			args.AppendAuxiliaryAttribute(sMatchWinnerKey) = matchWinner;

			Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
			worldState.mWorld->Enqueue(*e, worldState, 0);
		}
	}
}
