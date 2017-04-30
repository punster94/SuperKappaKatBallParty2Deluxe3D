
#include "pch.h"
#include "ActionRoundWinner.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionRoundWinner)

const string ActionRoundWinner::sPlayerIDKey = "playerID";

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
		Score* highScore = nullptr;
		uint32_t highScoreNum = 0;

		for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
		{
			Score* currScore = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
			if(currScore->GetScore() >= highScoreNum)
			{
				highScoreNum = currScore->GetScore();
				highScore = currScore;
			}
		}

		assert(highScore != nullptr);
		highScore->UpdateNumWins();

		// TODO -- post event if 3 rounds won
	}
}
