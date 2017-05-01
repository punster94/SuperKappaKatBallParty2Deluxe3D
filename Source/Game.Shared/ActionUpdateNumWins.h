
#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionUpdateNumWins final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionUpdateNumWins, FieaGameEngine::Action)

	public:

		explicit ActionUpdateNumWins(const std::string& name = "");
		virtual ~ActionUpdateNumWins();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		//

	public:

		//
	};

	ConcreteFactory(FieaGameEngine::Action, ActionUpdateNumWins);
}
