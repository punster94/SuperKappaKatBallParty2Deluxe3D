#include "pch.h"

#include "Timer.h"
#include "WorldState.h"
#include "HUD.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(Timer)

	void Timer::Initialize(float time, float posX, float posY, float width, float height)
	{
		mTimeRemaining = time;
		mPosX = posX;
		mPosY = posY;
		mWidth = width;
		mHeight = height;

		SetTimerPosition();
		SetTimerRenderables();
	}

	void Timer::Update(WorldState& state)
	{
		mTimeRemaining -= state.DeltaTime();

		SetTimerRenderables();
	}
	
	void Timer::Render(Renderer* renderer)
	{
		for (auto& it : mTimerRenderables)
		{
			it.Render(renderer);
		}
	}
	
	void Timer::SetTimerPosition()
	{
		Quad background;
		background.SetTexture(Asset::Get(TEXTURE_MANKEY_BALL_PNG)->As<Texture>());
		background.SetRect(mPosX, mPosY, mWidth, mHeight);
		background.SetColor(0, 0, 0, 1.0f);
		Quad firstDigit;
		firstDigit.SetRect(mPosX + .2f, mPosY, .1f, .2f);
		background.SetColor(1.0f, 0, 0, 1.0f);
		Quad secondDigit;
		secondDigit.SetRect(mPosX + .1f, mPosY, .1f, .2f);
		background.SetColor(0, 1.0f, 0, 1.0f);
		Quad thirdDigit;
		thirdDigit.SetRect(mPosX, mPosY, .1f, .2f);
		background.SetColor(0, 0, 1.0f, 1.0f);

		mTimerRenderables.PushBack(background);
		mTimerRenderables.PushBack(firstDigit);
		mTimerRenderables.PushBack(secondDigit);
		mTimerRenderables.PushBack(thirdDigit);

		VertexShader* vs = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
		PixelShader* ps = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
		for (auto& it : mTimerRenderables)
		{
			it.SetShaders(vs, ps);
		}
	}

	void Timer::SetTimerRenderables()
	{
		uint32_t time = mTimeRemaining;
		for (uint32_t i = 1; i < mTimerRenderables.Size(); ++i)
		{
			mTimerRenderables[i].SetTexture(Asset::Get(HUD::sNumbersVector[time % 10])->As<Texture>());
			time /= 10;
		}
	}
}