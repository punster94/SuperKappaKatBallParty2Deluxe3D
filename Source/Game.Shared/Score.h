
#pragma once

#include "RTTI.h"
#include "Quad.h"

namespace KatBall
{
	class Score final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Score, FieaGameEngine::RTTI)

	public:

		Score();
		virtual ~Score();

		void Initialize(const glm::vec4& color, float x, float y, float w, float h);
		void Update(FieaGameEngine::WorldState& worldState);
		void Render(FieaGameEngine::Renderer* renderer);

		void UpdateScore();

	private:

		void SetDigitTextures();

		std::uint32_t mScore;

		FieaGameEngine::Vector<FieaGameEngine::Quad*> mRenderables;
	};
}