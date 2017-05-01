
#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionResetRound final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionResetRound, FieaGameEngine::Action)

	public:

		explicit ActionResetRound(const std::string& name = "");
		virtual ~ActionResetRound();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		static const std::uint32_t sNoMatchWinnerFlag;
		static const std::uint32_t sTieGameFlag;

	public:

		static const std::string sMatchWinnersKey;
	};

	ConcreteFactory(FieaGameEngine::Action, ActionResetRound);
}
