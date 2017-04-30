
#include "pch.h"
#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(HUD)

const string HUD::sScoreDimensionsXKey = "score_dimensions_x";
const string HUD::sScoreDimensionsYKey = "score_dimensions_y";

const string HUD::sScoreColorsKey = "score_colors";
const string HUD::sScoreLocationsXKey = "score_locations_x";
const string HUD::sScoreLocationYKey = "score_location_y";

const string HUD::sScoresKey = "scores";

const string HUD::sTimerDimensionsXKey = "timer_dimensions_x";
const string HUD::sTimerDimensionsYKey = "timer_dimensions_y";

const string HUD::sTimerLocationXKey = "timer_location_x";
const string HUD::sTimerLocationYKey = "timer_location_y";
const string HUD::sTimeRemainingKey = "time_remaining";
const string HUD::sTimerColorKey = "timer_color";

const string HUD::sTimerKey = "timer";

const std::string HUD::sNumbersIcons[] =
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

const std::string HUD::sPlayerIcons[] =
{
	TEXTURE_PLAYER_ICON_ZERO,
	TEXTURE_PLAYER_ICON_ONE,
	TEXTURE_PLAYER_ICON_TWO,
	TEXTURE_PLAYER_ICON_THREE
};

const std::string HUD::sPlayerWinsIcons[] =
{
	TEXTURE_SCORE_BAR,
	TEXTURE_SCORE_BAR_ONE_WIN,
	TEXTURE_SCORE_BAR_TWO_WINS
};

HUD::HUD(const string& name) :
	Entity(name),
	mTimer(nullptr)
{
	InitializeSignatures();

	mTimer = new Timer();

	// create score objects -- stored as rtti's so we can get them via scope search
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		mScores[i] = new Score();
	}
}

HUD::~HUD()
{
	delete mTimer;

	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		delete mScores[i];
	}
}

void HUD::InitializeSignatures()
{
	Entity::InitializeSignatures();

	// init score attributes
	AddExternalAttribute(sScoreDimensionsXKey, &mScoreDimensionsX, 1);
	AddExternalAttribute(sScoreDimensionsYKey, &mScoreDimensionsY, 1);

	AddExternalAttribute(sScoreColorsKey, mScoreColors, NUM_PLAYERS);
	AddExternalAttribute(sScoreLocationsXKey, mScoreLocationsX, NUM_PLAYERS);
	AddExternalAttribute(sScoreLocationYKey, &mScoreLocationY, NUM_PLAYERS);

	AddExternalAttribute(sScoresKey, mScores, NUM_PLAYERS);

	// init timer attributes
	AddExternalAttribute(sTimerDimensionsXKey, &mTimerDimensionsX, 1);
	AddExternalAttribute(sTimerDimensionsYKey, &mTimerDimensionsY, 1);

	AddExternalAttribute(sTimerLocationXKey, &mTimerLocationX, 1);
	AddExternalAttribute(sTimerLocationYKey, &mTimerLocationY, 1);
	AddExternalAttribute(sTimeRemainingKey, &mTimeRemaining, 1);
	AddExternalAttribute(sTimerColorKey, &mTimerColor, 1);

	AddExternalAttribute(sTimerKey, &mTimer, 1);
}

void HUD::Initialize(WorldState& worldState)
{
	Entity::Initialize(worldState);

	static_cast<Timer*>(mTimer)->Initialize(
		mTimerColor,			// timer color
		mTimeRemaining,			// time remaining
		mTimerLocationX,		// origin point
		mTimerLocationY,
		mTimerDimensionsX,		// quad dimensions
		mTimerDimensionsY
	);

	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		static_cast<Score*>(mScores[i])->Initialize(
			sPlayerIcons[i],
			mScoreColors[i],			// color for this score
			mScoreLocationsX[i],		// origin point for this score
			mScoreLocationY,
			mScoreDimensionsX,			// quad dimensions
			mScoreDimensionsY
		);
	}
}

void HUD::Update(WorldState& worldState)
{
	Entity::Update(worldState);

	static_cast<Timer*>(mTimer)->Update(worldState);
	
	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		static_cast<Score*>(mScores[i])->Update(worldState);
	}
}

void HUD::Render(Renderer* renderer)
{
	Entity::Render(renderer);

	// turning off depth testing to prevent z-fighting
	renderer->SetDepthTesting(false);

	static_cast<Timer*>(mTimer)->Render(renderer);

	for(uint32_t i = 0; i < NUM_PLAYERS; ++i)
	{
		static_cast<Score*>(mScores[i])->Render(renderer);
	}

	renderer->SetDepthTesting(true);
}
