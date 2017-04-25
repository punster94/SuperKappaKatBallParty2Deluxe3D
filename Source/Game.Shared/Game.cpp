#include "pch.h"
#include "Game.h"
#include "Kat.h"
#include "TestDummy.h"
#include "Entity.h"
#include "Camera.h"
#include "Asset.h"
#include "Constants.h"

using namespace FieaGameEngine;

namespace KatBall
{
	static TestDummy* sDummy;
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
		LoadAssets();

		sDummy = new TestDummy();
		sCamera = new Camera();
		sCamera->SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));
		mRenderer->SetCamera(sCamera);

		if (mBackgroundMusic.openFromFile("Retribution.ogg"))
		{
			mBackgroundMusic.setLoop(true);
			mBackgroundMusic.play();
		}
	}

	void Game::Update()
	{
		mGameClock.UpdateGameTime(mWorldState.mGameTime);

		mWorld.Update(mWorldState);

		 // DEBUG
		sDummy->Update(mWorldState);
		// END

		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);
		
		// DEBUG
		sDummy->Render(mRenderer);
		// END

		mRenderer->EndRenderFrame();

		// DEBUG
		DebugUpdate();
		// END
	}

	void Game::DebugUpdate()
	{
		mWorldState.GetGameTime();

		float deltaTime = mWorldState.DeltaTime();

		const float cameraSpeed = 2.0f * deltaTime;
		const float cameraAngSpeed = 2.0f * deltaTime;
		glm::vec3 deltaPos;
		glm::vec3 deltaRot;
		glm::vec3 cameraPos = sCamera->GetPosition();
		glm::vec3 cameraRot = sCamera->GetRotation();

		if (GetAsyncKeyState('A'))
		{
			deltaPos += glm::vec3(-cameraSpeed, 0.0f, 0.0f);
		}

		if (GetAsyncKeyState('D'))
		{
			deltaPos += glm::vec3(cameraSpeed, 0.0f, 0.0f);
		}

		if (GetAsyncKeyState('S'))
		{
			deltaPos += glm::vec3(0.0f, 0.0f, -cameraSpeed);
		}

		if (GetAsyncKeyState('W'))
		{
			deltaPos += glm::vec3(0.0f, 0.0f, cameraSpeed);
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			deltaRot += glm::vec3(0.0f, -cameraAngSpeed, 0.0f);
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			deltaRot += glm::vec3(0.0f, cameraAngSpeed, 0.0f);
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			deltaRot += glm::vec3(cameraAngSpeed, 0.0f, 0.0f);
		}

		if (GetAsyncKeyState(VK_UP))
		{
			deltaRot += glm::vec3(-cameraAngSpeed, 0.0f, 0.0f);
		}

		sCamera->SetPosition(cameraPos + deltaPos);
		sCamera->SetRotation(cameraRot + deltaRot);
	}

	void Game::Shutdown()
	{
		mRenderer->Shutdown();
	}

	void Game::LoadAssets()
	{
		// Meshes
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_KAT, MESH_KAT, Asset::TYPE_MESH);
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_FLAT_SPHERE, MESH_FLAT_SPHERE, Asset::TYPE_MESH);
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_CUBE, MESH_CUBE, Asset::TYPE_MESH);

		// Textures
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_KAT, TEXTURE_KAT, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_MANKEY_BALL, TEXTURE_MANKEY_BALL, Asset::TYPE_TEXTURE);

		// Vertex Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_VERTEX, SHADER_MESH_VERTEX, Asset::TYPE_VERTEX_SHADER);

		// Pixel Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_PIXEL, SHADER_MESH_PIXEL, Asset::TYPE_PIXEL_SHADER);
	}
}