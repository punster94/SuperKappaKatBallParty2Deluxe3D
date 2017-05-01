
#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionRemoveCrowns final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionRemoveCrowns, FieaGameEngine::Action)

	public:

		explicit ActionRemoveCrowns(const std::string& name = "");
		~ActionRemoveCrowns();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		//
	};

	ConcreteFactory(FieaGameEngine::Action, ActionRemoveCrowns);
}