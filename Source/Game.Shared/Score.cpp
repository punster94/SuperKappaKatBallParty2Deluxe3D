
#include "pch.h"
#include "Score.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Score)

Score::Score()
{
	// populate renderables vector -- doing so here prevents a double add
	mRenderables.PushBack(new Quad());		// image
	mRenderables.PushBack(new Quad());		// ones place
	mRenderables.PushBack(new Quad());		// tens place
	mRenderables.PushBack(new Quad());		// score bar
}

Score::~Score()
{
	for(auto& renderable : mRenderables)
	{
		delete renderable;
	}
}

void Score::Initialize(const string& playerIcon, const vec4& color, float x, float y, float w, float h)
{
	// init quads
	mRenderables[0]->SetTexture(Asset::Get(playerIcon)->As<Texture>());
	mRenderables[0]->SetRect(x, y, w, h);
	//mRenderables[0]->SetColor(color);

	mRenderables[1]->SetRect(x + w + w, y, w, h);
	mRenderables[1]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[2]->SetRect(x + w, y, w, h);
	mRenderables[2]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[3]->SetTexture(Asset::Get(HUD::sPlayerWinsIcons[mNumWins])->As<Texture>());
	mRenderables[3]->SetRect(x + w, y + h, 2*w, h/2);
	mRenderables[3]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// set shaders
	VertexShader* vertShader = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
	PixelShader* pixShader = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
	for(auto& renderable : mRenderables)
	{
		renderable->SetShaders(vertShader, pixShader);
	}

	// init score and set digits
	mScore = 0;
	SetDigitTextures();
}

void Score::Update(WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);
	SetDigitTextures();
}

void Score::Render(Renderer* renderer)
{
	for(auto& renderable : mRenderables)
	{
		renderable->Render(renderer);
	}
}

void Score::SetDigitTextures()
{
	uint32_t tempScore = mScore;
	for(uint32_t i = 1; i < mRenderables.Size()-1; ++i)
	{
		mRenderables[i]->SetTexture(Asset::Get(HUD::sNumbersIcons[tempScore % 10])->As<Texture>());
		tempScore /= 10;
	}
}

void Score::UpdateScore()
{
	++mScore;
}
