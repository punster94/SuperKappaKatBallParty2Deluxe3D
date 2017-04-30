
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

		void SetDigitTextures();
		void UpdateScore();

	private:

		std::uint32_t mScore;

		FieaGameEngine::Quad mImageQuad;
		FieaGameEngine::Quad mDigitTens;
		FieaGameEngine::Quad mDigitOnes;

		FieaGameEngine::Vector<FieaGameEngine::Quad> mRenderables;
	};
}
