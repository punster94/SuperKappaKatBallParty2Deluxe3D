#include "pch.h"

using namespace FieaGameEngine;

namespace KatBall
{
	static TestDummy* sDummy;
	static Camera* sCamera;
	static Gamepad* sGamepad1;
	static Gamepad* sGamepad2;

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

		mWorld.Initialize();

		sDummy = new TestDummy();
		sCamera = new Camera();
		sGamepad1 = new Gamepad(0);
		sGamepad2 = new Gamepad(1);
		sCamera->SetPosition(glm::vec3(0.0f, 0.0f, -12.0f));
		mRenderer->SetCamera(sCamera);

		mBackgroundMusic.SetMusicFile("Retribution.ogg");
		mBackgroundMusic.Play();
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

		// DEBUG
		float lAnalogY;
		float rAnalogY;
		float lAnalogX;
		float rAnalogX;
		if (sGamepad1->Refresh())
		{
			if (sGamepad1->IsPressed(XINPUT_GAMEPAD_A))
			{
				lAnalogY = sGamepad1->leftStickY;
				rAnalogY = sGamepad1->rightStickY;
				lAnalogX = sGamepad1->leftStickX;
				rAnalogX = sGamepad1->rightStickX;
			}
		}

		if (sGamepad2->Refresh())
		{
			if (sGamepad2->IsPressed(XINPUT_GAMEPAD_A))
			{
				lAnalogY = sGamepad2->leftStickY;
				rAnalogY = sGamepad2->rightStickY;
				lAnalogX = sGamepad2->leftStickX;
				rAnalogX = sGamepad2->rightStickX;
			}
		}
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