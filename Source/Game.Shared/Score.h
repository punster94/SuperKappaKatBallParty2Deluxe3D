
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
		void Reset();

		void Update(FieaGameEngine::WorldState& worldState);
		void Render(FieaGameEngine::Renderer* renderer);

		void UpdateNumWins();
		void UpdateScore();

		void SetIsWinning(bool isWinning);
		std::uint32_t GetNumWins() const;
		std::uint32_t GetScore() const;

	private:

		void SetDigitsTexture();
		void SetBarTexture();

		std::uint32_t mNumWins;
		std::uint32_t mScore;
		bool mIsWinning;

		FieaGameEngine::Vector<FieaGameEngine::Quad*> mRenderables;

		static const std::uint32_t sOnesIndex;
		static const std::uint32_t sTensIndex;
		static const std::uint32_t sBarIndex;
		static const std::uint32_t sImageIndex;
		static const std::uint32_t sCrownIndex;
	};
}
