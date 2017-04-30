
#pragma once

#include "Entity.h"
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
		virtual void Update(FieaGameEngine::WorldState& worldState) override;
		virtual void Render(FieaGameEngine::Renderer* renderer) override;

	protected:

		void InitializeSignatures();

	private:

		float mQuadDimensionsX;
		float mQuadDimensionsY;

		glm::vec4 mScoreColors[NUM_PLAYERS];
		float mScoreLocationsX[NUM_PLAYERS];
		float mScoreLocationY;

		RTTI* mScores[NUM_PLAYERS];

	public:

		static const std::string sQuadDimensionsXKey;
		static const std::string sQuadDimensionsYKey;
		
		static const std::string sScoreColorsKey;
		static const std::string sScoreLocationsXKey;
		static const std::string sScoreLocationYKey;

		static const std::string sScoresKey;

		const static std::string sNumbersVector[];
	};

	ConcreteFactory(FieaGameEngine::Entity, HUD);
}
