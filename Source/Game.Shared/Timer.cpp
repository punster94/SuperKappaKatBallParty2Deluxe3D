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

	const std::string Timer::sTimeUpEventSubtype = "timeup";
	const float Timer::sNumberOffsetLeft = .05f;
	const float Timer::sNumberOffsetBottom = .1f;
	const float Timer::sNumberScaleFactor = .1f;

	Timer::Timer()
	{
		// populate renderables vector -- doing so here prevents a double add
		mTimerRenderables.PushBack(new Quad());		// background
		mTimerRenderables.PushBack(new Quad());		// ones place
		mTimerRenderables.PushBack(new Quad());		// tens place
		mTimerRenderables.PushBack(new Quad());		// hundreds place
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
		// init quads
		mTimerRenderables[0]->SetTexture(Asset::Get(TEXTURE_TIMER_BACKGROUND)->As<Texture>());
		//mTimerRenderables[0]->SetRect(posX - 0.025f, posY - 0.025f, width * 3 + 0.05f, height + 0.05f);
		mTimerRenderables[0]->SetRect(posX, posY, width, height);
		mTimerRenderables[0]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

		mTimerRenderables[1]->SetRect(posX + 2*sNumberOffsetLeft, posY + sNumberOffsetBottom, width * sNumberScaleFactor, height * sNumberScaleFactor * 1.5f);
		mTimerRenderables[1]->SetColor(color);

		mTimerRenderables[2]->SetRect(posX + sNumberOffsetLeft, posY + sNumberOffsetBottom, width* sNumberScaleFactor, height* sNumberScaleFactor * 1.5f);
		mTimerRenderables[2]->SetColor(color);

		mTimerRenderables[3]->SetRect(posX, posY + sNumberOffsetBottom, width* sNumberScaleFactor, height* sNumberScaleFactor * 1.5f);
		mTimerRenderables[3]->SetColor(color);

		// set shaders
		VertexShader* vs = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
		PixelShader* ps = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
		for(auto& it : mTimerRenderables)
		{
			it->SetShaders(vs, ps);
		}
		
		// init time, run, and set digits
		mTimeRemaining = mDefaultTimeRemaining = time;
		SetTimerRenderables();

		mIsRunning = true;
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
				Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(EventMessageAttributed(sTimeUpEventSubtype, &state));
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
		for (uint32_t i = 1; i < mTimerRenderables.Size(); ++i)
		{
			mTimerRenderables[i]->SetTexture(Asset::Get(HUD::sNumbersIcons[time % 10])->As<Texture>());
			time /= 10;
		}
	}
}