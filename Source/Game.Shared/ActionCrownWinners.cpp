
#include "pch.h"
#include "ActionCrownWinners.h"

#include "Sector.h"
#include "ActionResetRound.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionCrownWinners)

ActionCrownWinners::ActionCrownWinners(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ActionCrownWinners::~ActionCrownWinners()
{
}

void ActionCrownWinners::Update(class WorldState& worldState)
{
	if(Datum* winners = Search(ActionResetRound::sMatchWinnersKey))
	{
		if(Datum* entities = Search(Sector::sSectorEntitiesKey))
		{
			for(uint32_t i = 0; i < entities->Size(); ++i)
			{
				// find the current dood in the winners list
				bool isWinner = false;
				for(uint32_t j = 0; j < winners->Size(); ++j)
				{
					if(winners->Get<int32_t&>(j) == i)
					{
						isWinner = true;
						break;
					}
				}

				// add or remove crown if winner or not
				if(isWinner)
				{
					static_cast<QuadEntity*>(&entities->Get<Scope&>(i))->AddQuadToView();
				}
				else
				{
					static_cast<QuadEntity*>(&entities->Get<Scope&>(i))->RemoveQuadFromView();
				}
			}
		}
	}
}
