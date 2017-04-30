
#pragma once

#include "Action.h"

namespace KatBall
{
	class ActionRoundWinner final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(ActionRoundWinner, FieaGameEngine::Action)

	public:

		explicit ActionRoundWinner(const std::string& name = "");
		virtual ~ActionRoundWinner();

		virtual void Update(class FieaGameEngine::WorldState& worldState) override;

	private:

		//

	public:

		static const std::string sMatchWonEventSubtype;
		static const std::string sMatchWinnerIDKey;

		static const std::uint32_t sNoMatchWinnerFlag;
		static const std::uint32_t sTieGameFlag;
	};

	ConcreteFactory(FieaGameEngine::Action, ActionRoundWinner);
}
