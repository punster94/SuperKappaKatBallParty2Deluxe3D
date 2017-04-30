
#include "pch.h"
#include "Score.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(Score)

Score::Score()
{
}

Score::~Score()
{
}

void Score::Initialize(WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);

	mScore = 0;

	// TODO -- init location, dimensions, texture
}

void Score::Render(Renderer* renderer)
{
	UNREFERENCED_PARAMETER(renderer);

	// TODO -- render quads
}

void Score::Update()
{
	++mScore;
}
