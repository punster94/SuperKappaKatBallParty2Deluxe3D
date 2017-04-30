
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

		void Initialize(const std::string& playerIcon, const glm::vec4& color, float x, float y, float w, float h);
		void Update(FieaGameEngine::WorldState& worldState);
		void Render(FieaGameEngine::Renderer* renderer);

		void UpdateScore();

		void SetIsWinning(bool isWinning);
		std::uint32_t GetScore() const;

	private:

		void SetDigitTextures();

		std::uint32_t mScore;
		std::uint32_t mNumWins;
		bool mIsWinning;
		FieaGameEngine::Vector<FieaGameEngine::Quad*> mRenderables;
	};
}
