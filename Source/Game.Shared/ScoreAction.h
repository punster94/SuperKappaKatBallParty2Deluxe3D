
#pragma once

#include "Action.h"

namespace KatBall
{
	class ScoreAction final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ScoreAction, FieaGameEngine::Action)
		
	public:

		explicit ScoreAction(const std::string& name = "");
		virtual ~ScoreAction();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		//

	public:

		static const std::string sScoreEventSubtype;
		static const std::string sPlayerIDKey;
	};

	ConcreteFactory(FieaGameEngine::Action, ScoreAction);
}
