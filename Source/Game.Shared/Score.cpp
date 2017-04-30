
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
}

Score::~Score()
{
}

void Score::Initialize(const vec4& color, float x, float y, float w, float h)
{
	// init image quad
	mImageQuad.SetTexture(Asset::Get(TEXTURE_KAT_SCORE_IMAGE)->As<Texture>());
	mImageQuad.SetRect(x, y, w, h);
	mImageQuad.SetColor(color);

	// init tens quad
	mDigitTens.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	mDigitTens.SetRect(x + w, y, w, h);

	// init ones quad
	mDigitOnes.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	mDigitOnes.SetRect(x + w + w, y, w, h);

	// populate renderables vector
	mRenderables.PushBack(mImageQuad);
	mRenderables.PushBack(mDigitOnes);
	mRenderables.PushBack(mDigitTens);

	// set quad shaders
	VertexShader* vertShader = Asset::Get(SHADER_QUAD_VERTEX)->As<VertexShader>();
	PixelShader* pixShader = Asset::Get(SHADER_QUAD_PIXEL)->As<PixelShader>();
	for(auto& renderable : mRenderables)
	{
		renderable.SetShaders(vertShader, pixShader);
	}

	// init score and set digit textures
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
		renderable.Render(renderer);
	}
}

void Score::SetDigitTextures()
{
	uint32_t tempScore = mScore;
	for(uint32_t i = 1; i < mRenderables.Size(); ++i)
	{
		mRenderables[i].SetTexture(Asset::Get(HUD::sNumbersVector[tempScore % 10])->As<Texture>());
		tempScore /= 10;
	}
}

void Score::UpdateScore()
{
	++mScore;
}
