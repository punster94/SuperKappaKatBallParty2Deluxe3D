#include "pch.h"

#include "Timer.h"
#include "WorldState.h"
#include "HUD.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(Timer)

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
		mTimerRenderables[0]->SetTexture(Asset::Get(TEXTURE_MANKEY_BALL_PNG)->As<Texture>());
		mTimerRenderables[0]->SetRect(posX - 0.025f, posY - 0.025f, width * 3 + 0.05f, height + 0.05f);
		mTimerRenderables[0]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

		mTimerRenderables[1]->SetRect(posX + width + width, posY, width, height);
		mTimerRenderables[1]->SetColor(color);

		mTimerRenderables[2]->SetRect(posX + width, posY, width, height);
		mTimerRenderables[2]->SetColor(color);

		mTimerRenderables[3]->SetRect(posX, posY, width, height);
		mTimerRenderables[3]->SetColor(color);

		// set shaders
		VertexShader* vs = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
		PixelShader* ps = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
		for(auto& it : mTimerRenderables)
		{
			it->SetShaders(vs, ps);
		}
		
		// init time and set digits
		mTimeRemaining = time;
		SetTimerRenderables();
	}

	void Timer::Update(WorldState& state)
	{
		mTimeRemaining -= state.DeltaTime();
		SetTimerRenderables();
	}
	
	void Timer::Render(Renderer* renderer)
	{
		// turning off depth testing to prevent z-fighting with background
		renderer->SetDepthTesting(false);
		for (auto& it : mTimerRenderables)
		{
			it->Render(renderer);
		}

		renderer->SetDepthTesting(true);
	}

	void Timer::SetTimerRenderables()
	{
		uint32_t time = mTimeRemaining;
		for (uint32_t i = 1; i < mTimerRenderables.Size(); ++i)
		{
			mTimerRenderables[i]->SetTexture(Asset::Get(HUD::sNumbersVector[time % 10])->As<Texture>());
			time /= 10;
		}
	}
}