
#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionCrownWinners final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionCrownWinners, FieaGameEngine::Action)

	public:

		explicit ActionCrownWinners(const std::string& name = "");
		~ActionCrownWinners();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		//
	};

	ConcreteFactory(FieaGameEngine::Action, ActionCrownWinners);
}
