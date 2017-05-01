#pragma once
#include "Entity.h"
#include "Gamepad.h"

namespace KatBall
{
	class MenuGamepad final : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(MenuGamepad, FieaGameEngine::Entity)

	public:

		explicit MenuGamepad(const std::string& name = "");
		virtual ~MenuGamepad();
		MenuGamepad(const MenuGamepad& otherPlayer);
		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;
		virtual Scope* Copy() const override;
		virtual void Update(FieaGameEngine::WorldState& worldState) override;
		
		void DeleteGamepad() { delete mGamepad; mGamepad = nullptr; }

	protected:
		void InitializeSignatures();

	private:

		void CopyPrivateDataMembers(const MenuGamepad& otherPlayer);
		void FixExternalAttributes();

		Gamepad* mGamepad;

		const static std::string sStartGameEventSubtype;
	};

	ConcreteEntityFactory(MenuGamepad);
}