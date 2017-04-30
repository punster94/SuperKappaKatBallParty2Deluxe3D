
#include "pch.h"
#include "Score.h"

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

void Score::Initialize(const std::string& numbersFilePath, const std::string& imageFilePath, const vec4& color, float x, float y, float dimension)
{
	UNREFERENCED_PARAMETER(numbersFilePath);
	UNREFERENCED_PARAMETER(imageFilePath);
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(dimension);

	mScore = 0;

	// TODO -- init location, dimensions, texture
}

void Score::Render(Renderer* renderer)
{
	UNREFERENCED_PARAMETER(renderer);

	// TODO -- render quads
}

void Score::Update()
{
	++mScore;
}
