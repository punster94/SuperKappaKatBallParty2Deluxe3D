
#pragma once

#include "RTTI.h"

namespace KatBall
{
	class Score final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Score, FieaGameEngine::RTTI)

	public:

		Score();
		virtual ~Score();

		void Initialize(FieaGameEngine::WorldState& worldState);
		void Update(FieaGameEngine::WorldState& worldState);
		void Render(FieaGameEngine::Renderer* renderer);

	private:

		//

	public:

		//
	};
}
