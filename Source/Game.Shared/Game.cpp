#include "pch.h"
#include "Game.h"
#include "SFML/Audio.hpp"

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
		mMeshTest = &mRenderer->CreateMesh("");

		if (mBackgroundMusic.openFromFile("Retribution.ogg"))
		{
			mBackgroundMusic.setLoop(true);
			mBackgroundMusic.play();
		}
	}

	void Game::Update()
	{
		mWorld.Update(mWorldState);
		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);
		mMeshTest->Render(mRenderer);
		mRenderer->EndRenderFrame();
	}

	void Game::Shutdown()
	{
		delete mMeshTest;
		mRenderer->Shutdown();
	}
}
