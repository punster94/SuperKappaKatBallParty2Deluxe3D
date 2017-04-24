#include "pch.h"
#include "Game.h"
#include "Kat.h"
#include "Entity.h"

using namespace FieaGameEngine;

namespace KatBall
{
	static Kat* sKat;

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
		sKat = new Kat();
	}

	void Game::Update()
	{
		mWorld.Update(mWorldState);
		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);
		sKat->As<Entity>()->Render(mRenderer);
		mRenderer->EndRenderFrame();
	}

	void Game::Shutdown()
	{
		delete mMeshTest;
		mRenderer->Shutdown();
	}
}