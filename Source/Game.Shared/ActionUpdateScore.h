
#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionUpdateScore final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionUpdateScore, FieaGameEngine::Action)
		
	public:

		explicit ActionUpdateScore(const std::string& name = "");
		virtual ~ActionUpdateScore();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		//

	public:

		static const std::string sScoreEventSubtype;
		static const std::string sPlayerIDKey;
	};

	ConcreteFactory(FieaGameEngine::Action, ActionUpdateScore);
}
