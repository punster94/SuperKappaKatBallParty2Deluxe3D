
#include "pch.h"
#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(HUD)

const string HUD::sQuadDimensionsXKey = "quad_dimensions_x";
const string HUD::sQuadDimensionsYKey = "quad_dimensions_y";

const string HUD::sScoreColorsKey = "score_colors";
const string HUD::sScoreLocationsXKey = "score_locations_x";
const string HUD::sScoreLocationYKey = "score_location_y";

const string HUD::sScoresKey = "scores";

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
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		mScores[i] = new Score();
	}
}

HUD::~HUD()
{
	// delete score objects
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		delete mScores[i];
	}
}

void HUD::InitializeSignatures()
{
	Entity::InitializeSignatures();

	AddExternalAttribute(sQuadDimensionsXKey, &mQuadDimensionsX, 1);
	AddExternalAttribute(sQuadDimensionsYKey, &mQuadDimensionsY, 1);

	AddExternalAttribute(sScoreColorsKey, mScoreColors, NUM_PLAYERS);
	AddExternalAttribute(sScoreLocationsXKey, mScoreLocationsX, NUM_PLAYERS);
	AddExternalAttribute(sScoreLocationYKey, &mScoreLocationY, NUM_PLAYERS);

	AddExternalAttribute(sScoresKey, mScores, NUM_PLAYERS);

	// TODO
}

void HUD::Initialize(WorldState& worldState)
{
	Entity::Initialize(worldState);

	// TODO

	// initialize scores
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		static_cast<Score*>(mScores[i])->Initialize(
			mScoreColors[i],			// color for this score
			mScoreLocationsX[i],		// origin point for this score
			mScoreLocationY,
			mQuadDimensionsX,			// quad dimensions
			mQuadDimensionsY
		);
	}
}

void HUD::Update(WorldState& worldState)
{
	Entity::Update(worldState);

	// TODO

	// update scores
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		static_cast<Score*>(mScores[i])->Update(worldState);
	}
}

void HUD::Render(Renderer* renderer)
{
	Entity::Render(renderer);

	// TODO

	// render scores
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		static_cast<Score*>(mScores[i])->Render(renderer);
	}
}
