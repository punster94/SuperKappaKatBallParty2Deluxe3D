
#include "pch.h"
#include "Score.h"

#include "HUD.h"

using namespace FieaGameEngine;
using namespace KatBall;
using namespace std;
using namespace glm;

RTTI_DEFINITIONS(Score)

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
	mRenderables[0]->SetTexture(Asset::Get(playerIcon)->As<Texture>());
	mRenderables[0]->SetRect(x + w + w, y, w, h);
	mRenderables[0]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[1]->SetRect(x + w, y, w, h);
	mRenderables[1]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[2]->SetTexture(Asset::Get(HUD::sPlayerWinsIcons[mNumWins])->As<Texture>());
	mRenderables[2]->SetRect(x + w, y + h, w * 2.0f, h * 0.5f);
	mRenderables[2]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	mRenderables[3]->SetTexture(Asset::Get(playerIcon)->As<Texture>());
	mRenderables[3]->SetRect(x, y, w, h);

	mRenderables[4]->SetTexture(Asset::Get(TEXTURE_CROWN)->As<Texture>());
	mRenderables[4]->SetRect(x + w * 0.25f, y + h * 0.75f, w * 0.5f, h * 0.5f);
	mRenderables[4]->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	// set shaders
	VertexShader* vertShader = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
	PixelShader* pixShader = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
	for(auto& renderable : mRenderables)
	{
		renderable->SetShaders(vertShader, pixShader);
	}

	// init score and set digits
	mScore = 10;
	SetDigitTextures();

	mIsWinning = false;
}

void Score::Update(WorldState& worldState)
{
	UNREFERENCED_PARAMETER(worldState);
	SetDigitTextures();
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

void Score::SetDigitTextures()
{
	uint32_t tempScore = mScore;

	for(uint32_t i = 0; i < 2; ++i)
	{
		mRenderables[i]->SetTexture(Asset::Get(HUD::sNumbersIcons[tempScore % 10])->As<Texture>());
		tempScore /= 10;
	}
}

void Score::UpdateNumWins()
{
	++mNumWins;
	if (mNumWins < ROUNDS_TO_WIN)
	{
		mRenderables[2]->SetTexture(Asset::Get(HUD::sPlayerWinsIcons[mNumWins])->As<Texture>());
	}
}

void Score::UpdateScore()
{
	++mScore;
}

void KatBall::Score::Reset()
{
	mScore = 0;
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
