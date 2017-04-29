
#pragma once

#include "Entity.h"

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

	protected:

		void InitializeSignatures();

	private:

		std::int32_t mScores[NUM_HUD_SLOTS];
		glm::vec4 mLocations[NUM_HUD_SLOTS];
		float mDimensions[NUM_HUD_SLOTS];

	public:

		static const std::string sScoresKey;
		static const std::string sLocationsKey;
		static const std::string sDimensionsKey;
		static const std::string sPlayerIDKey;

		static const std::string sScoreEventSubtype;
	};
}
