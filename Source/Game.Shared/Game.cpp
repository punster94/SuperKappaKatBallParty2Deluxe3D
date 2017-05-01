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
		mGameClock.UpdateGameTime(mWorldState.mGameTime);

		mWorld.Update(mWorldState);

		mRenderer->InitRenderFrame();
		mRenderer->Render(mWorld);

		mRenderer->EndRenderFrame();
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

		// Vertex Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_VERTEX, SHADER_MESH_VERTEX, Asset::TYPE_VERTEX_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_QUAD_VERTEX, SHADER_QUAD_VERTEX, Asset::TYPE_VERTEX_SHADER);

		// Pixel Shaders
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_MESH_PIXEL, SHADER_MESH_PIXEL, Asset::TYPE_PIXEL_SHADER);
		Asset::Load(ASSET_DIRECTORY_SHADERS SHADER_QUAD_PIXEL, SHADER_QUAD_PIXEL, Asset::TYPE_PIXEL_SHADER);
	}
}