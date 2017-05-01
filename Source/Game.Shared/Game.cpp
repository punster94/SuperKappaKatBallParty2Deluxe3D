#include "pch.h"
#include <experimental/filesystem>

#include "InputSubscriber.h"

#include "EventMessageAttributed.h"
#include "Event.h"

#include "HUD.h"
#include "ActionResetRound.h"
#include "ActionUpdateScore.h"
#include "ActionUpdateNumWins.h"
#include "ReactionAttributed.h"
#include "Timer.h"
#include "KatMusic.h"
#include "KatSound.h"
#include "Menu.h"
#include "MenuGamepad.h"
#include "PowerupSpawner.h"
#include "ActionFocus.h"

using namespace FieaGameEngine;

namespace KatBall
{
	static InputSubscriber* sInputSubscriber;
	static Quad* sQuad;

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

	WorldState& Game::GetWorldState()
	{
		return mWorldState;
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

		mGameSector = new Sector("Game");
		mMenuSector = new Sector("Menu");
		mMenuSector->SetWorld(mWorld);

		mStateManager.Initialize(mWorld, *mMenuSector, *mGameSector, mWorldState);

		ScopeParseHelper::ScopeSharedData sharedData;
		XmlParseMaster master(&sharedData);
		ScopeParseHelper helper;

		master.AddHelper(&helper);

		EntityFactory ef;
		CameraFactory cf;
		RigidBodyFactory rbf;
		KatMusicFactory kmf;
		KatSoundFactory ksf;
		MeshEntityFactory mef;
		HUDFactory hudf;
		ActionResetRoundFactory arrf;
		ActionUpdateScoreFactory ausf;
		ActionUpdateNumWinsFactory aunwf;
		ActionFocusFactory aff;
		ReactionAttributedFactory raf;
		QuadEntityFactory qef;
		PlayerFactory pf;
		MenuFactory mf;
		MenuGamepadFactory mgf;
		PowerupSpawnerFactory psf;
		PowerupFactory pwf;

		std::experimental::filesystem::directory_iterator menuEntities(ASSET_DIRECTORY_MENU_ENTITIES);

		for (std::experimental::filesystem::directory_entry path : menuEntities)
		{
			master.ParseFromFile(path.path().string());

			Entity* entity = sharedData.mScope->As<Entity>();
			sharedData.mScope = nullptr;

			entity->SetSector(*mMenuSector);
		}

		std::experimental::filesystem::directory_iterator gameEntities(ASSET_DIRECTORY_GAME_ENTITIES);

		for(std::experimental::filesystem::directory_entry path : gameEntities)
		{
			master.ParseFromFile(path.path().string());

			Entity* entity = sharedData.mScope->As<Entity>();
			sharedData.mScope = nullptr;

			entity->SetSector(*mGameSector);
		}

		mWorld.Initialize(mWorldState);
	
		sInputSubscriber = new InputSubscriber();
	}

	void Game::Update()
	{
		mDynamicsWorld->stepSimulation(1.f / 60.f, 10);

		mGameClock.UpdateGameTime(mWorldState.mGameTime);

		mWorld.Update(mWorldState);

		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);

		mRenderer->EndRenderFrame();

		Entity* objectA;
		Entity* objectB;

		Player* player = nullptr;
		Player* player2 = nullptr;
		Powerup* powerUp = nullptr;

		RigidBody* rigidbody = nullptr;

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
			powerUp = objectB->As<Powerup>();
			player2 = objectB->As<Player>();
			rigidbody = objectB->As<RigidBody>();

			if (player != nullptr && powerUp != nullptr)
			{
				powerUp->OnCollect(*player);
				shouldDeletePowerUp = true;
			}

			if (player != nullptr && player2!= nullptr)
			{
				player->SetLastPlayerTouching(player2->GetPlayerID());
				player2->SetLastPlayerTouching(player->GetPlayerID());
			}

			if (player != nullptr && rigidbody != nullptr)
			{
				player2 = rigidbody->GetParent()->As<Player>();
				player->SetLastPlayerTouching(player2->GetPlayerID());
				player2->SetLastPlayerTouching(player->GetPlayerID());
				player->OnHit();
			}
		}

		if (shouldDeletePowerUp)
		{
			delete powerUp;
		}
	}

	void Game::RegisterRigidBody(btCollisionShape& shape, btRigidBody& body)
	{
		mDynamicsWorld->addRigidBody(&body);
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
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_PLAYER_ICON_ZERO, TEXTURE_PLAYER_ICON_ZERO, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_PLAYER_ICON_ONE, TEXTURE_PLAYER_ICON_ONE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_PLAYER_ICON_TWO, TEXTURE_PLAYER_ICON_TWO, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_PLAYER_ICON_THREE, TEXTURE_PLAYER_ICON_THREE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_SCORE_BAR, TEXTURE_SCORE_BAR, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_SCORE_BAR_ONE_WIN, TEXTURE_SCORE_BAR_ONE_WIN, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_SCORE_BAR_TWO_WINS, TEXTURE_SCORE_BAR_TWO_WINS, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_TIMER_BACKGROUND, TEXTURE_TIMER_BACKGROUND, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_KAT_SCORE_IMAGE, TEXTURE_KAT_SCORE_IMAGE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_CROWN, TEXTURE_CROWN, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_START_SCREEN, TEXTURE_START_SCREEN, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_START_BUTTON, TEXTURE_START_BUTTON, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_START_BUTTON_HIGHLIGHT, TEXTURE_START_BUTTON_HIGHLIGHT, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_PAUSE_BUTTON, TEXTURE_PAUSE_BUTTON, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_TIMER, TEXTURE_TIMER, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_TIMER_BURN_LINE, TEXTURE_TIMER_BURN_LINE, Asset::TYPE_TEXTURE);
		Asset::Load(ASSET_DIRECTORY_TEXTURES TEXTURE_TIMER_BURN, TEXTURE_TIMER_BURN, Asset::TYPE_TEXTURE);

		// Vertex Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_VERTEX, SHADER_MESH_VERTEX, Asset::TYPE_VERTEX_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_QUAD_VERTEX, SHADER_QUAD_VERTEX, Asset::TYPE_VERTEX_SHADER);

		// Pixel Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_PIXEL, SHADER_MESH_PIXEL, Asset::TYPE_PIXEL_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_QUAD_PIXEL, SHADER_QUAD_PIXEL, Asset::TYPE_PIXEL_SHADER);
	}

	Game* Game::sInstance = nullptr;
}