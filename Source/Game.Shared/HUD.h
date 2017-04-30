
#pragma once

#include "Entity.h"
#include "Score.h"

namespace KatBall
{
# define NUM_HUD_SLOTS 4

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

		float mQuadDimensions;
		std::string mNumbersFile;

		std::string mScoreImageFile;
		glm::vec4 mScoreColors[NUM_HUD_SLOTS];
		float mScoreLocationsX[NUM_HUD_SLOTS];
		float mScoreLocationsY[NUM_HUD_SLOTS];

		RTTI* mScores[NUM_HUD_SLOTS];

	public:

		static const std::string sQuadDimensionsKey;
		static const std::string sNumbersFileKey;
		
		static const std::string sScoreColorsKey;
		static const std::string sScoreImageFileKey;
		static const std::string sScoreLocationsXKey;
		static const std::string sScoreLocationsYKey;

		static const std::string sScoresKey;
	};

	ConcreteFactory(FieaGameEngine::Entity, HUD);
}
