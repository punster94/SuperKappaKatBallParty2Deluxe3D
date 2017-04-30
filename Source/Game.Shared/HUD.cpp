
#include "pch.h"
#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(HUD)

const string HUD::sScoreLocationsKey = "score_locations";
const string HUD::sQuadDimensionsKey = "quad_dimensions";

const string HUD::sPlayerIDKey = "playerID";
const string HUD::sScoresKey = "scores";

const string HUD::sScoreEventSubtype = "score";
const std::string HUD::sNumbersVector[] =
{
	TEXTURE_NUMBER_ZERO,
	TEXTURE_NUMBER_ONE,
	TEXTURE_NUMBER_TWO,
	TEXTURE_NUMBER_THREE,
	TEXTURE_NUMBER_FOUR,
	TEXTURE_NUMBER_FIVE,
	TEXTURE_NUMBER_SIX,
	TEXTURE_NUMBER_SEVEN,
	TEXTURE_NUMBER_EIGHT,
	TEXTURE_NUMBER_NINE
};

HUD::HUD(const string& name) :
	Entity(name)
{
	InitializeSignatures();

	// create score objects -- stored as rtti's so we can get them via scope search
	for(uint32_t i = 0; i < NUM_HUD_SLOTS; ++i)
	{
		mScores[i] = new Score();
	}
}

HUD::~HUD()
{
	// delete score objects
	for(uint32_t i = 0; i < NUM_HUD_SLOTS; ++i)
	{
		delete mScores[i];
	}
}

void HUD::InitializeSignatures()
{
	Entity::InitializeSignatures();

	AddExternalAttribute(sQuadDimensionsKey, &mQuadDimensions, 1);
	AddExternalAttribute(sScoreLocationsKey, mScoreLocations, NUM_HUD_SLOTS);

	AddExternalAttribute(sScoresKey, mScores, NUM_HUD_SLOTS);

	// TODO
}

void HUD::Initialize(WorldState& worldState)
{
	Entity::Initialize(worldState);

	// TODO

	// initialize scores
	for(uint32_t i = 0; i < NUM_HUD_SLOTS; ++i)
	{
		static_cast<Score*>(mScores[i])->Initialize(worldState);
	}
}

void HUD::Update(WorldState& worldState)
{
	Entity::Update(worldState);

	// TODO

	// update scores
	for(uint32_t i = 0; i < NUM_HUD_SLOTS; ++i)
	{
		static_cast<Score*>(mScores[i])->Update(worldState);
	}
}

void HUD::Render(Renderer* renderer)
{
	Entity::Render(renderer);

	// TODO

	// render scores
	for(uint32_t i = 0; i < NUM_HUD_SLOTS; ++i)
	{
		static_cast<Score*>(mScores[i])->Render(renderer);
	}
}
