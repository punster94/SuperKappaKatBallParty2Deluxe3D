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
		void Initialize(float time, float posX, float posY, float width, float height);
		void Update(class FieaGameEngine::WorldState& state);
		void Render(FieaGameEngine::Renderer* renderer);

	private:
		void SetTimerPosition();
		void SetTimerRenderables();

		float mTimeRemaining;
		float mPosX;
		float mPosY;
		float mWidth;
		float mHeight;
		FieaGameEngine::Vector<FieaGameEngine::Quad> mTimerRenderables;
	};
}