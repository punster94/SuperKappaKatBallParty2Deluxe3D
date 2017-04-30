#pragma once

#include "Quad.h"
#include "Renderer.h"
#include "RTTI.h"

namespace KatBall
{
	class Timer : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Timer, FieaGameEngine::RTTI)

	public:
		Timer();
		virtual ~Timer();

		void Initialize(const glm::vec4& color, float time, float posX, float posY, float width, float height);
		void Update(class FieaGameEngine::WorldState& state);
		void Render(FieaGameEngine::Renderer* renderer);

	private:

		void SetTimerRenderables();

		float mDefaultTimeRemaining;
		float mTimeRemaining;

		bool mIsRunning;

		FieaGameEngine::Vector<FieaGameEngine::Quad*> mTimerRenderables;

	public:

		static const std::string sTimeUpEventSubtype;
		static const float sNumberOffsetLeft;
		static const float sNumberOffsetBottom;
		static const float sNumberScaleFactor;
	};
}