
#pragma once

#include "Entity.h"

namespace KatBall
{
	class MenuCrowns final : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(MenuCrowns, FieaGameEngine::Entity)

	public:

		explicit MenuCrowns(const std::string& name = "");
		~MenuCrowns();

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;
		virtual void Reset(FieaGameEngine::WorldState& worldState) override;

	private:

		//
	};

	ConcreteFactory(FieaGameEngine::Entity, MenuCrowns);
}
