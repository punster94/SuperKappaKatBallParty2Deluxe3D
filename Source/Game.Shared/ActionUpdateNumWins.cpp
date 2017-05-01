
#include "pch.h"
#include "ActionUpdateNumWins.h"

#include "HUD.h"
#include "Score.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionUpdateNumWins)

ActionUpdateNumWins::ActionUpdateNumWins(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ActionUpdateNumWins::~ActionUpdateNumWins()
{
}

void ActionUpdateNumWins::Update(class WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);

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

		// update players that have the high score
		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(score->GetScore() == highScore)
			{
				score->UpdateNumWins();
			}
		}
	}
}
