
#include "pch.h"
#include "MenuCrowns.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(MenuCrowns)

MenuCrowns::MenuCrowns(const string& name) :
	Entity(name)
{
	InitializeSignatures();
}

MenuCrowns::~MenuCrowns()
{
}

void MenuCrowns::Initialize(WorldState& worldState)
{
	Entity::Initialize(worldState);

	Datum& entities = Entities();
	for(uint32_t i = 0; i < entities.Size(); ++i)
	{
		static_cast<QuadEntity*>(&entities.Get<Scope&>(i))->RemoveQuadFromView();
	}
}

void MenuCrowns::Reset(WorldState& worldState)
{
	Datum& entities = Entities();
	for(uint32_t i = 0; i < entities.Size(); ++i)
	{
		static_cast<QuadEntity*>(&entities.Get<Scope&>(i))->RemoveQuadFromView();
	}
}
