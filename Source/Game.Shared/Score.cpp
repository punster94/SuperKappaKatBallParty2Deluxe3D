
#include "pch.h"
#include "Score.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Score)

const uint32_t Score::sOnesIndex = 0;
const uint32_t Score::sTensIndex = 1;
const uint32_t Score::sBarIndex = 2;
const uint32_t Score::sImageIndex = 3;
const uint32_t Score::sCrownIndex = 4;

Score::Score() :
	mScore(0),
	mNumWins(0)
{
	// populate renderables vector -- doing so here prevents a double add
	mRenderables.PushBack(new Quad());		// ones place
	mRenderables.PushBack(new Quad());		// tens place
	mRenderables.PushBack(new Quad());		// score bar
	mRenderables.PushBack(new Quad());		// image
	mRenderables.PushBack(new Quad());		// crown
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
	mRenderables[sOnesIndex]->SetTexture(Asset::Get(playerIcon)->As<Texture>());
	mRenderables[sOnesIndex]->SetRect(x + w + w, y, w, h);
	mRenderables[sOnesIndex]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[sTensIndex]->SetRect(x + w, y, w, h);
	mRenderables[sTensIndex]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[sBarIndex]->SetRect(x + w, y + h, w * 2.0f, h * 0.5f);
	mRenderables[sBarIndex]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[sImageIndex]->SetTexture(Asset::Get(playerIcon)->As<Texture>());
	mRenderables[sImageIndex]->SetRect(x, y, w, h);

	mRenderables[sCrownIndex]->SetTexture(Asset::Get(TEXTURE_CROWN)->As<Texture>());
	mRenderables[sCrownIndex]->SetRect(x + w * 0.25f, y + h * 0.75f, w * 0.5f, h * 0.5f);
	mRenderables[sCrownIndex]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// set shaders
	VertexShader* vertShader = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
	PixelShader* pixShader = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
	for(auto& renderable : mRenderables)
	{
		renderable->SetShaders(vertShader, pixShader);
	}

	// init score and set digits
	mNumWins = 0;
	Reset();
}

void KatBall::Score::Reset()
{
	mIsWinning = false;
	mScore = 0;

	SetDigitsTexture();
	SetBarTexture();
}

void Score::Update(WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);
}

void Score::Render(Renderer* renderer)
{
	// we only draw the last quad (crown) if we're flagged as winning
	uint32_t lastQuad = mIsWinning ? mRenderables.Size() : mRenderables.Size() - 1;
	for(uint32_t i = 0; i < lastQuad; ++i)
	{
		mRenderables[i]->Render(renderer);
	}
}

void Score::UpdateNumWins()
{
	++mNumWins;
	SetBarTexture();
}

void Score::UpdateScore()
{
	++mScore;
	SetDigitsTexture();
}

void Score::SetIsWinning(bool isWinning)
{
	mIsWinning = isWinning;
}

uint32_t Score::GetNumWins() const
{
	return mNumWins;
}

uint32_t Score::GetScore() const
{
	return mScore;
}

void Score::SetDigitsTexture()
{
	uint32_t tempScore = mScore;
	for(uint32_t i = 0; i < 2; ++i)
	{
		mRenderables[i]->SetTexture(Asset::Get(HUD::sNumbersIcons[tempScore % 10])->As<Texture>());
		tempScore /= 10;
	}
}

void Score::SetBarTexture()
{
	if(mNumWins < ROUNDS_TO_WIN)
	{
		mRenderables[sBarIndex]->SetTexture(Asset::Get(HUD::sPlayerWinsIcons[mNumWins])->As<Texture>());
	}
}
