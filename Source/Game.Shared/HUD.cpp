
#include "pch.h"
#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(HUD)

const string HUD::sQuadDimensionsKey = "quad_dimensions";
const string HUD::sNumbersFileKey = "numbers_file";

const string HUD::sScoreColorsKey = "score_colors";
const string HUD::sScoreImageFileKey = "score_image_file";
const string HUD::sScoreLocationsXKey = "score_x_locations";
const string HUD::sScoreLocationsYKey = "score_y_locations";

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
	AddExternalAttribute(sNumbersFileKey, &mNumbersFile, 1);

	AddExternalAttribute(sScoreImageFileKey, &mScoreImageFile, 1);
	AddExternalAttribute(sScoreColorsKey, mScoreColors, NUM_HUD_SLOTS);
	AddExternalAttribute(sScoreLocationsXKey, mScoreLocationsX, NUM_HUD_SLOTS);
	AddExternalAttribute(sScoreLocationsYKey, mScoreLocationsY, NUM_HUD_SLOTS);

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
		static_cast<Score*>(mScores[i])->Initialize(
			mNumbersFile,				// file for numbers texture
			mScoreImageFile,			// file for score image texture
			mScoreColors[i],			// color for this score
			mScoreLocationsX[i],		// origin x for this score
			mScoreLocationsY[i],		// origin y for this score
			mQuadDimensions				// quad dimensions
		);
	}
}

void HUD::Update(WorldState& worldState)
{
	Entity::Update(worldState);

	// TODO

	// NOTE -- not updating scores here (update increments score)
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
