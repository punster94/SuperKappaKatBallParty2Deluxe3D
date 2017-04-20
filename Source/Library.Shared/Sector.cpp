#include "pch.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector(const std::string& name) :
		Attributed(), mName(name), mWorld(nullptr)
	{
		InitializeSignatures();
	}

	Sector::Sector(const Sector& otherSector) :
		Attributed(otherSector), mName(otherSector.mName), mWorld(nullptr)
	{
		CopyPrivateDataMembers(otherSector);
	}

	Sector& Sector::operator=(const Sector& otherSector)
	{
		if (this != &otherSector)
		{
			Attributed::operator=(otherSector);

			CopyPrivateDataMembers(otherSector);
		}

		return *this;
	}

	Sector::Sector(Sector&& previousSector) :
		Attributed(std::move(previousSector)), mName(previousSector.mName), mWorld(previousSector.mWorld)
	{
		CopyPrivateDataMembers(previousSector);

		SetWorld(*previousSector.mWorld);

		previousSector.mWorld = nullptr;
		previousSector.mName = std::string();
	}

	Sector& Sector::operator=(Sector&& previousSector)
	{
		if (this != &previousSector)
		{
			Attributed::operator=(std::move(previousSector));

			CopyPrivateDataMembers(previousSector);

			SetWorld(*previousSector.mWorld);

			previousSector.mWorld = nullptr;
			previousSector.mName = std::string();
		}

		return *this;
	}

	const std::string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string& name)
	{
		mName = name;
	}

	World* Sector::GetWorld() const
	{
		return mWorld;
	}

	void Sector::SetWorld(World& world)
	{
		world.Adopt(*this, World::sWorldSectorsKey);
		mWorld = &world;
	}

	Datum& Sector::Entities()
	{
		return const_cast<Datum&>(const_cast<const Sector*>(this)->Entities());
	}

	const Datum& Sector::Entities() const
	{
		const Datum* datum = Find(sSectorEntitiesKey);
		assert(datum != nullptr);
		return *datum;
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		Entity* entity = Factory<Entity>::Create(className);
		entity->SetName(instanceName);
		entity->SetSector(*this);

		return entity;
	}

	Datum& Sector::Actions()
	{
		return const_cast<Datum&>(const_cast<const Sector*>(this)->Actions());
	}

	const Datum& Sector::Actions() const
	{
		const Datum* datum = Find(Action::sActionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	Action* Sector::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		action->SetName(instanceName);
		Adopt(*action, Action::sActionsKey);

		return action;
	}

	Datum& Sector::Reactions()
	{
		return const_cast<Datum&>(const_cast<const Sector*>(this)->Reactions());
	}

	const Datum& Sector::Reactions() const
	{
		const Datum* datum = Find(Reaction::sReactionsKey);
		assert(datum != nullptr);
		return *datum;
	}

	void Sector::Update(WorldState& worldState)
	{
		worldState.mSector = this;

		Datum& actions = Actions();

		for (std::uint32_t i = 0; i < actions.Size(); ++i)
		{
			static_cast<Action*>(&actions.Get<Scope&>(i))->Update(worldState);
		}

		Datum& entities = Entities();

		for (std::uint32_t i = 0; i < entities.Size(); ++i)
		{
			static_cast<Entity*>(&entities.Get<Scope&>(i))->Update(worldState);
		}
	}

	Scope* Sector::Copy() const
	{
		return new Sector(*this);
	}

	void Sector::InitializeSignatures()
	{
		Attributed::InitializeSignatures();

		AddExternalAttribute(sSectorNameKey, &mName, 1);

		AddEmptyTable(sSectorEntitiesKey);
		AddEmptyTable(Action::sActionsKey);
		AddEmptyTable(Reaction::sReactionsKey);
	}

	void Sector::CopyPrivateDataMembers(const Sector& otherSector)
	{
		mName = otherSector.mName;
		mWorld = nullptr;

		FixExternalAttributes();
	}

	void Sector::FixExternalAttributes()
	{
		Append(sSectorNameKey).SetStorage(&mName, 1);
	}

	const std::string Sector::sSectorNameKey = "name";

	const std::string Sector::sSectorEntitiesKey = "entities";
}