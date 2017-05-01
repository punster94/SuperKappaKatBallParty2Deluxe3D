#include "pch.h"

#include "Menu.h"
#include "RendererDirectX.h"
#include "MenuGamepad.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(Menu)

	const std::string Menu::sAlternateTextureFilenameKey = "alternate texture filename";

	Menu::Menu(const std::string& name) :
		QuadEntity(name)
	{
		InitializeSignatures();
		Event<Gamepad>::Subscribe(*this);
	}

	Menu::~Menu()
	{
		Event<Gamepad>::Unsubscribe(*this);
	}

	Menu::Menu(const Menu& other) :
		QuadEntity(other)
	{
		CopyPrivateDataMembers(other);
	}

	void Menu::Initialize(WorldState& worldState)
	{
		QuadEntity::Initialize(worldState);
	}

	Scope* Menu::Copy() const
	{
		return new Menu(*this);
	}

	void Menu::Notify(const FieaGameEngine::EventPublisher& eventPublisher)
	{
		assert(eventPublisher.Is(Event<Gamepad>::TypeIdClass()));

		if (Game::GetInstance()->GetWorldState().mSector->Name() != "Game")
		{
			RemoveQuadFromView();

			Entity* entity = FindChildEntityByName("menu gamepad");
			if (entity != nullptr)
			{
				MenuGamepad* gamepad = entity->As<MenuGamepad>();
				gamepad->DeleteGamepad();
			}
		}
	}

	void Menu::InitializeSignatures()
	{
		QuadEntity::InitializeSignatures();

		AddEmptyString(sAlternateTextureFilenameKey);
	}

	void Menu::CopyPrivateDataMembers(const Menu& other)
	{
		mTextureFileName = other.mTextureFileName;
		mVertexShaderFileName = other.mVertexShaderFileName;
		mPixelShaderFileName = other.mPixelShaderFileName;
		mColor = other.mColor;
		mRect = other.mRect;
	}

	void Menu::FixExternalAttributes()
	{
		Append(sTextureFileNameKey).SetStorage(&mTextureFileName, 1);
		Append(sVertexShaderFileNameKey).SetStorage(&mVertexShaderFileName, 1);
		Append(sPixelShaderFileNameKey).SetStorage(&mPixelShaderFileName, 1);
		Append(sColorKey).SetStorage(&mColor, 1);
		Append(sRectKey).SetStorage(&mRect, 1);
	}
}