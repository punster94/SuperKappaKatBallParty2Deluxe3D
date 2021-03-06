#include "pch.h"

#include "Timer.h"
#include "WorldState.h"
#include "HUD.h"

#include "EventMessageAttributed.h"
#include "Event.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(Timer)

	const std::string Timer::sRoundOverEventSubtype = "round_over";
	const float Timer::sNumberOffsetLeft = .05f;
	const float Timer::sNumberOffsetBottom = .1f;
	const float Timer::sNumberScaleFactor = .2f;
	const float Timer::sTimerWidth = .25f;

	Timer::Timer()
	{
		// populate renderables vector -- doing so here prevents a double add
		mTimerRenderables.PushBack(new Quad());		// background
		mTimerRenderables.PushBack(new Quad());		// ones place
		mTimerRenderables.PushBack(new Quad());		// tens place
		mTimerRenderables.PushBack(new Quad());		// hundreds place
		mTimerRenderables.PushBack(new Quad());		// timer burn line
		mTimerRenderables.PushBack(new Quad());		// timer burn star
	}

	Timer::~Timer()
	{
		for(auto& it : mTimerRenderables)
		{
			delete it;
		}
	}

	void Timer::Initialize(const glm::vec4& color, float time, float posX, float posY, float width, float height)
	{
		mBurnPosX = posX;
		mBurnPosY = posY;
		mHeight = height;

		// init quads
		mTimerRenderables[0]->SetTexture(Asset::Get(TEXTURE_TIMER)->As<Texture>());
		mTimerRenderables[0]->SetRect(posX, posY, sTimerWidth, height);
		mTimerRenderables[0]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

		mTimerRenderables[1]->SetRect(posX + 2*sNumberOffsetLeft, posY + sNumberOffsetBottom, sTimerWidth * sNumberScaleFactor, height * sNumberScaleFactor * 1.5f);
		mTimerRenderables[1]->SetColor(color);

		mTimerRenderables[2]->SetRect(posX + sNumberOffsetLeft, posY + sNumberOffsetBottom, sTimerWidth* sNumberScaleFactor, height* sNumberScaleFactor * 1.5f);
		mTimerRenderables[2]->SetColor(color);

		mTimerRenderables[3]->SetRect(posX, posY + sNumberOffsetBottom, sTimerWidth* sNumberScaleFactor, height* sNumberScaleFactor * 1.5f);
		mTimerRenderables[3]->SetColor(color);

		mTimerRenderables[4]->SetTexture(Asset::Get(TEXTURE_TIMER_BURN_LINE)->As<Texture>());
		mTimerRenderables[4]->SetRect(mBurnPosX + sTimerWidth, mBurnPosY, 1.0f, mHeight);
		mTimerRenderables[4]->SetColor(color);

		mTimerRenderables[5]->SetTexture(Asset::Get(TEXTURE_TIMER_BURN)->As<Texture>());
		mTimerRenderables[5]->SetRect(mBurnPosX + sTimerWidth + 1.0f, mBurnPosY, .04f, mHeight);
		mTimerRenderables[5]->SetColor(color);

		// set shaders
		VertexShader* vs = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
		PixelShader* ps = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
		for(auto& it : mTimerRenderables)
		{
			it->SetShaders(vs, ps);
		}
		
		// init time, run, and set digits
		mDefaultTimeRemaining = time;
		Reset();
	}

	void Timer::Reset()
	{
		mTimeRemaining = mDefaultTimeRemaining;
		mIsRunning = true;

		SetTimerRenderables();
	}

	void Timer::Update(WorldState& state)
	{
		if(mIsRunning)
		{
			mTimeRemaining -= state.DeltaTime();
			SetTimerRenderables();

			// post timeup event and stop running timer
			if(mTimeRemaining <= 0.0f)
			{
				Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(EventMessageAttributed(sRoundOverEventSubtype, &state));
				state.mWorld->Enqueue(*e, state, 0);

				mTimeRemaining = mDefaultTimeRemaining;
				mIsRunning = false;
			}
		}
	}
	
	void Timer::Render(Renderer* renderer)
	{
		for (auto& it : mTimerRenderables)
		{
			it->Render(renderer);
		}
	}

	void Timer::SetTimerRenderables()
	{
		uint32_t time = mTimeRemaining;
		for (uint32_t i = 1; i < 1 + sNumberDigits; ++i)
		{
			mTimerRenderables[i]->SetTexture(Asset::Get(HUD::sNumbersIcons[time % 10])->As<Texture>());
			time /= 10;
		}

		mTimerRenderables[4]->SetRect(mBurnPosX + sTimerWidth, mBurnPosY, mTimeRemaining/mDefaultTimeRemaining, mHeight);
		mTimerRenderables[5]->SetRect(mBurnPosX + sTimerWidth + mTimeRemaining / mDefaultTimeRemaining, mBurnPosY, .04f, mHeight);
	}
}