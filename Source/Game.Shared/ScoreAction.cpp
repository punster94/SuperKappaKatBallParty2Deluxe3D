
#include "pch.h"
#include "ScoreAction.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ScoreAction)

ScoreAction::ScoreAction(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ScoreAction::~ScoreAction()
{
}

void ScoreAction::InitializeSignatures()
{
	Action::InitializeSignatures();

	// TODO
}

void ScoreAction::Update(WorldState& worldState)
{
	if(Datum* playerIDDatum = Search(HUD::sPlayerIDKey))
	{
		// assert that playerID is valid
		int32_t playerID = playerIDDatum->Get<int32_t&>();
		assert(playerID >= 0 && playerID < NUM_HUD_SLOTS);

		if(Datum* scoresDatum = Search(HUD::sScoresKey))
		{
			// increment score at playerID
			scoresDatum->Set(++scoresDatum->Get<int32_t&>(playerID), playerID);
		}
	}
}