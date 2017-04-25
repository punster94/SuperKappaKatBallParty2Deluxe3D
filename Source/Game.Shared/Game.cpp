#include "pch.h"
#include "Game.h"
#include "Kat.h"
#include "Entity.h"
#include "Camera.h"

using namespace FieaGameEngine;

namespace KatBall
{
	static Kat* sKat;
	static Camera* sCamera;

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
		sCamera = new Camera();
		sCamera->SetPosition(glm::vec3(0.0f, 0.0f, -10.0f));
		mRenderer->SetCamera(sCamera);
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
		mRenderer->Shutdown();
	}
}