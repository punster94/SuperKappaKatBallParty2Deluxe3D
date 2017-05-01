#include "pch.h"
#include <experimental/filesystem>

#include "InputSubscriber.h"

#include "HUD.h"
#include "ScoreAction.h"
#include "ReactionAttributed.h"
#include "Timer.h"
#include "KatMusic.h"
#include "KatSound.h"
#include "PowerupSpawner.h"

using namespace FieaGameEngine;

namespace KatBall
{
	static Camera* sCamera;
	static InputSubscriber* sInputSubscriber;

	Game::Game(FieaGameEngine::Renderer& renderer)
		: mRenderer(&renderer)
	{
		sInstance = this;
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
		mCollisionConfiguration = new btDefaultCollisionConfiguration();

		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

		mOverlappingPairCache = new btDbvtBroadphase();

		mSolver = new btSequentialImpulseConstraintSolver;

		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration);
		mDynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

		mRenderer->Init();
		LoadAssets();

		Sector* sector = mWorld.CreateSector("");

		ScopeParseHelper::ScopeSharedData sharedData;
		XmlParseMaster master(&sharedData);
		ScopeParseHelper helper;

		master.AddHelper(&helper);

		EntityFactory ef;
		RigidBodyFactory rbf;
		KatMusicFactory kmf;
		KatSoundFactory ksf;
		MeshEntityFactory mef;
		HUDFactory hudf;
		ScoreActionFactory saf;
		ReactionAttributedFactory raf;
		QuadEntityFactory qef;
		PlayerFactory pf;
		PowerupSpawnerFactory psf;
		PowerupFactory pwf;

		std::experimental::filesystem::directory_iterator directoryIt(ASSET_DIRECTORY_ENTITIES);

		for (std::experimental::filesystem::directory_entry path : directoryIt)
		{
			master.ParseFromFile(path.path().string());

			Entity* entity = sharedData.mScope->As<Entity>();
			sharedData.mScope = nullptr;

			entity->SetSector(*sector);
		}

		mWorld.Initialize(mWorldState);

		sCamera = new Camera();
		sInputSubscriber = new InputSubscriber();
		sCamera->SetRelativePosition(glm::vec3(0.0f, 10.0f, -12.0f));
		sCamera->SetRelativeRotation(glm::vec3(0.71f, 0.0f, 0.0f));
		mRenderer->SetCamera(sCamera);
	}

	void Game::Update()
	{
		mDynamicsWorld->stepSimulation(1.f / 60.f, 10);

		mGameClock.UpdateGameTime(mWorldState.mGameTime);

		mWorld.Update(mWorldState);

		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);

		mRenderer->EndRenderFrame();

		// DEBUG
		DebugUpdate();

		Entity* objectA;
		Entity* objectB;

		Player* player = nullptr;
		Powerup* powerUp = nullptr;

		bool shouldDeletePowerUp = false;

		int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			objectA = static_cast<Entity*>(obA->getUserPointer());
			objectB = static_cast<Entity*>(obB->getUserPointer());

			player = objectA->As<Player>();
			powerUp = objectB->As <Powerup>();

			if (player != nullptr && powerUp != nullptr)
			{
				powerUp->OnCollect(*player);
				shouldDeletePowerUp = true;
			}
		}

		if (shouldDeletePowerUp)
		{
			delete powerUp;
		}
		// END
	}

	void Game::RegisterRigidBody(btCollisionShape& shape, btRigidBody& body)
	{
		mDynamicsWorld->addRigidBody(&body);
	}

	void Game::DebugUpdate()
	{
		mWorldState.GetGameTime();

		float deltaTime = mWorldState.DeltaTime();

		const float cameraSpeed = 2.0f * deltaTime;
		const float cameraAngSpeed = 2.0f * deltaTime;
		glm::vec3 deltaPos;
		glm::vec3 deltaRot;
		glm::vec3 cameraPos = sCamera->GetWorldPosition();
		glm::vec3 cameraRot = sCamera->GetWorldRotation();

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

		if (GetAsyncKeyState('E'))
		{
			deltaPos += glm::vec3(0.0f, cameraSpeed, 0.0f);
		}

		if (GetAsyncKeyState('Q'))
		{
			deltaPos += glm::vec3(0.0f, -cameraSpeed, 0.0f);
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

		if (GetAsyncKeyState(VK_SPACE))
		{
			Datum& entities = mWorld.Sectors().Get<Scope&>(0).As<Sector>()->Entities();
			for (uint32_t i = 0; i < entities.Size(); ++i)
			{
				if (entities.Get<Scope&>(i).Is(KatSound::TypeIdClass()))
				{
					static_cast<KatSound&>(entities.Get<Scope&>(i)).Play();
					break;
				}
			}
		}

		sCamera->SetRelativePosition(cameraPos + deltaPos);
		sCamera->SetRelativeRotation(cameraRot + deltaRot);
	}

	void Game::Shutdown()
	{
		mRenderer->Shutdown();

		delete mDynamicsWorld;
		delete mSolver;
		delete mOverlappingPairCache;
		delete mDispatcher;
		delete mCollisionConfiguration;
	}

	Game* Game::GetInstance()
	{
		return sInstance;
	}

	void Game::LoadAssets()
	{
		// Meshes
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_KAT, MESH_KAT, Asset::TYPE_MESH);
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_FLAT_SPHERE, MESH_FLAT_SPHERE, Asset::TYPE_MESH);
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_CUBE, MESH_CUBE, Asset::TYPE_MESH);
		Asset::Load(ASSET_DIRECTORY_MESHES MESH_SMOOTH_SPHERE, MESH_SMOOTH_SPHERE, Asset::TYPE_MESH);

		// Textures
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_KAT, TEXTURE_KAT, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_MANKEY_BALL, TEXTURE_MANKEY_BALL, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_MANKEY_BALL_PNG, TEXTURE_MANKEY_BALL_PNG, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_ZERO, TEXTURE_NUMBER_ZERO, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_ONE, TEXTURE_NUMBER_ONE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_TWO, TEXTURE_NUMBER_TWO, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_THREE, TEXTURE_NUMBER_THREE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_FOUR, TEXTURE_NUMBER_FOUR, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_FIVE, TEXTURE_NUMBER_FIVE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_SIX, TEXTURE_NUMBER_SIX, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_SEVEN, TEXTURE_NUMBER_SEVEN, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_EIGHT, TEXTURE_NUMBER_EIGHT, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_NUMBER_NINE, TEXTURE_NUMBER_NINE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_KAT_SCORE_IMAGE, TEXTURE_KAT_SCORE_IMAGE, Asset::TYPE_TEXTURE);

		// Vertex Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_VERTEX, SHADER_MESH_VERTEX, Asset::TYPE_VERTEX_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_QUAD_VERTEX, SHADER_QUAD_VERTEX, Asset::TYPE_VERTEX_SHADER);

		// Pixel Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_PIXEL, SHADER_MESH_PIXEL, Asset::TYPE_PIXEL_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_QUAD_PIXEL, SHADER_QUAD_PIXEL, Asset::TYPE_PIXEL_SHADER);
	}

	Game* Game::sInstance = nullptr;
}