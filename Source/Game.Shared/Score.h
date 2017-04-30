
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

		void Initialize(const std::string& numbersFilePath, const std::string& imageFilePath, const glm::vec4& color, float x, float y, float dimension);
		void Render(FieaGameEngine::Renderer* renderer);

		void Update();

	private:

		std::uint32_t mScore;

		FieaGameEngine::Quad mImageQuad;
		FieaGameEngine::Vector<FieaGameEngine::Quad> mDigitQuads;
	};
}
