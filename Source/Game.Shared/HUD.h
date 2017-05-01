
#pragma once

#include "Entity.h"
#include "Timer.h"
#include "Score.h"

namespace KatBall
{
	class HUD final : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(HUD, FieaGameEngine::Entity)

	public:

		explicit HUD(const std::string& name = "");
		virtual ~HUD();

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;
		virtual void Reset(FieaGameEngine::WorldState& worldState) override;

		virtual void Update(FieaGameEngine::WorldState& worldState) override;
		virtual void Render(FieaGameEngine::Renderer* renderer) override;

	protected:

		void InitializeSignatures();

	private:

		float mScoreDimensionsX;
		float mScoreDimensionsY;

		glm::vec4 mScoreColors[NUM_PLAYERS];
		float mScoreLocationsX[NUM_PLAYERS];
		float mScoreLocationY;

		float mTimerDimensionsX;
		float mTimerDimensionsY;

		float mTimerLocationX;
		float mTimerLocationY;
		float mTimeRemaining;
		glm::vec4 mTimerColor;

		RTTI* mScores[NUM_PLAYERS];
		RTTI* mTimer;

	public:

		static const std::string sScoreDimensionsXKey;
		static const std::string sScoreDimensionsYKey;
		
		static const std::string sScoreColorsKey;
		static const std::string sScoreLocationsXKey;
		static const std::string sScoreLocationYKey;

		static const std::string sScoresKey;

		static const std::string sTimerDimensionsXKey;
		static const std::string sTimerDimensionsYKey;

		static const std::string sTimerLocationXKey;
		static const std::string sTimerLocationYKey;
		static const std::string sTimeRemainingKey;
		static const std::string sTimerColorKey;

		static const std::string sTimerKey;

		const static std::string sNumbersIcons[];
		const static std::string sPlayerIcons[];
		const static std::string sPlayerWinsIcons[];
	};

	ConcreteFactory(FieaGameEngine::Entity, HUD);
}
