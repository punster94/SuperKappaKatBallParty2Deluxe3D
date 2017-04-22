#include "pch.h"
#include "Game.h"

namespace KatBall
{
	void Game::Run()
	{
		Init();
		Update();
	}

	void Game::Init()
	{
		mRenderer->Init();
	}

	void Game::Update()
	{
		mWorld.Update(mWorldState);
		mRenderer->Render(mWorld);
	}
}