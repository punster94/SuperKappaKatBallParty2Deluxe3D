
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
		glm::vec4 mScoreLocations[NUM_HUD_SLOTS];

		RTTI* mScores[NUM_HUD_SLOTS];

	public:

		static const std::string sScoreLocationsKey;
		static const std::string sQuadDimensionsKey;

		static const std::string sPlayerIDKey;
		static const std::string sScoresKey;

		static const std::string sScoreEventSubtype;
		const static std::string sNumbersVector[];
	};

	ConcreteFactory(FieaGameEngine::Entity, HUD);
}
