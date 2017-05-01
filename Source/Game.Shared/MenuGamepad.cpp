#include "pch.h"

#include "MenuGamepad.h"
#include "Menu.h"
#include "EventMessageAttributed.h"

using namespace FieaGameEngine;

namespace KatBall
{
	RTTI_DEFINITIONS(MenuGamepad)

	const std::string MenuGamepad::sStartGameEventSubtype = "start_game";

	MenuGamepad::MenuGamepad(const std::string& name) :
		Entity(name), mGamepad(nullptr)
	{
		InitializeSignatures();
	}

	MenuGamepad::~MenuGamepad()
	{
		DeleteGamepad();
	}

	MenuGamepad::MenuGamepad(const MenuGamepad& other) :
		Entity(other)
	{
		CopyPrivateDataMembers(other);
	}

	void MenuGamepad::Initialize(WorldState& worldState)
	{
		Entity::Initialize(worldState);

		mGamepad = new Gamepad();
	}

	Scope* MenuGamepad::Copy() const
	{
		return new MenuGamepad(*this);
	}

	void MenuGamepad::Update(FieaGameEngine::WorldState& worldState)
	{
		worldState.mEntity = this;

		if (mGamepad->Refresh())
		{
			if (mGamepad->GetState()->wButtons != 0)
			{
				FieaGameEngine::Event<Gamepad>* event = new FieaGameEngine::Event<Gamepad>(*mGamepad, false);
				worldState.mWorld->Enqueue(*event, worldState, 0);

				EventMessageAttributed args(sStartGameEventSubtype, &worldState);
				Event<EventMessageAttributed>* e = new Event<EventMessageAttributed>(args);
				worldState.mWorld->Enqueue(*e, worldState, 0);
			}
		}
	}

	//void MenuGamepad::Notify(const FieaGameEngine::EventPublisher& eventPublisher)
	//{
	//	assert(GetParent()->Is(Sector::TypeIdClass()));
	//	Datum& entityDatum = static_cast<Sector*>(GetParent())->Entities();
	//	
	//	for (uint32_t i = 0; i < entityDatum.Size(); ++i)
	//	{
	//		Scope& current = entityDatum.Get<Scope&>(i);
	//		if (current.Is(Menu::TypeIdClass()))
	//		{
	//			static_cast<Menu&>(current)
	//		}
	//	}
	//}

	void MenuGamepad::CopyPrivateDataMembers(const MenuGamepad& other)
	{
		FixExternalAttributes();
	}

	void MenuGamepad::FixExternalAttributes()
	{
	}

	void MenuGamepad::InitializeSignatures()
	{
	}
}