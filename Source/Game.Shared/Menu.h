
#pragma once

#include "QuadEntity.h"
#include "Timer.h"
#include "Score.h"
#include "Event.h"
#include "EventSubscriber.h"

namespace KatBall
{
	class Menu final : public QuadEntity, public FieaGameEngine::EventSubscriber
	{
		RTTI_DECLARATIONS(Menu, QuadEntity)

	private:
		static const std::uint32_t sMaxNumberOptions = 10;

	public:

		explicit Menu(const std::string& name = "");
		virtual ~Menu();
		Menu(const Menu& otherQuadEntity);

		virtual void Initialize(FieaGameEngine::WorldState& worldState) override;
		virtual Scope* Copy() const override;
		void Notify(const FieaGameEngine::EventPublisher& eventPublisher) override;
		void Reset(FieaGameEngine::WorldState& worldState) override;

	protected:

		void InitializeSignatures();

	private:
		void CopyPrivateDataMembers(const Menu& other);

		void FixExternalAttributes();

		static const std::string sAlternateTextureFilenameKey;
		
	};

	ConcreteEntityFactory(Menu);
}
