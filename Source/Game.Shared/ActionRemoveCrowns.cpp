
#include "pch.h"
#include "ActionRemoveCrowns.h"

#include "Sector.h"
#include "ActionResetRound.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(ActionRemoveCrowns)

ActionRemoveCrowns::ActionRemoveCrowns(const string& name) :
	Action(name)
{
	InitializeSignatures();
}

ActionRemoveCrowns::~ActionRemoveCrowns()
{
}

void ActionRemoveCrowns::Update(class WorldState& worldState)
{
	if(Datum* entities = Search(Sector::sSectorEntitiesKey))
	{
		for(uint32_t i = 0; i < entities->Size(); ++i)
		{
			static_cast<QuadEntity*>(&entities->Get<Scope&>(i))->RemoveQuadFromView();
		}
	}
}
