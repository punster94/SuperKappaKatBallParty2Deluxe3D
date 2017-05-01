
#include "pch.h"
#include "ActionUpdateScore.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionUpdateScore)

const string ActionUpdateScore::sScoreEventSubtype = "score";
const string ActionUpdateScore::sPlayerIDKey = "playerID";

ActionUpdateScore::ActionUpdateScore(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ActionUpdateScore::~ActionUpdateScore()
{
}

void ActionUpdateScore::Update(WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);

	Datum* playerIDDatum = Search(sPlayerIDKey);
	Datum* scoresDatum = Search(HUD::sScoresKey);
	if(playerIDDatum != nullptr && scoresDatum != nullptr)
	{
		// check if playerID is valid
		int32_t playerID = playerIDDatum->Get<int32_t&>();
		if(playerID >= 0 && playerID < NUM_PLAYERS)
		{
			// get the scores from HUD and update using player ID
			static_cast<Score*>(scoresDatum->Get<RTTI*&>(playerID))->UpdateScore();

			// get high score
			uint32_t highScore = 0;
			for(uint32_t i = 0; i < scoresDatum->Size(); ++i)
			{
				uint32_t currScore = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i))->GetScore();
				if(highScore < currScore)
				{
					highScore = currScore;
				}
			}

			// crown leader
			for(uint32_t i = 0; i < scoresDatum->Size(); ++i)
			{
				Score* score = static_cast<Score*>(scoresDatum->Get<RTTI*&>(i));
				score->SetIsWinning(score->GetScore() == highScore);
			}
		}
	}
}
