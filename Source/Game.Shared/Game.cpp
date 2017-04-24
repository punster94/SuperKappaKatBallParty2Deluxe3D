#include "pch.h"
#include "Game.h"

namespace KatBall
{
	Game::Game(FieaGameEngine::Renderer& renderer)
		: mRenderer(&renderer)
	{
	}
	
	void Game::Run()
	{
		Init();

		while (mRenderer->IsValid())
		{
			Update();
		}

		Shutdown();
	}

	void Game::Init()
	{
		mRenderer->Init();
	}

	void Game::Update()
	{
		mWorld.Update(mWorldState);
		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);
		mRenderer->EndRenderFrame();
	}

	void Game::Shutdown()
	{
		delete mMeshTest;
		mRenderer->Shutdown();
	}
}