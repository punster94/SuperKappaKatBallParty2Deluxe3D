
#include "pch.h"
#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;

RTTI_DEFINITIONS(HUD)

const string HUD::sScoresKey = "scores";
const string HUD::sLocationsKey = "locations";
const string HUD::sDimensionsKey = "dimensions";
const string HUD::sPlayerIDKey = "playerID";

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
}

HUD::~HUD()
{
}

void HUD::InitializeSignatures()
{
	Entity::InitializeSignatures();

	AddExternalAttribute(sScoresKey, mScores, NUM_HUD_SLOTS);
	AddExternalAttribute(sLocationsKey, mLocations, NUM_HUD_SLOTS);
	AddExternalAttribute(sDimensionsKey, &mDimensions, 1);

	// TODO -- attributes for textures and stuff?
}

void HUD::Initialize(WorldState& worldState)
{
	Entity::Initialize(worldState);

	// init scores to 0
	for(uint32_t i = 0; i < NUM_HUD_SLOTS; ++i)
	{
		mScores[i] = 0;
	}

	// TODO -- add / init renderables
}
